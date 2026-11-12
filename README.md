# SDCC with fixes for mcs51

This is a fork of [SDCC](https://sourceforge.net/projects/sdcc/). Branch `main` is SDCC's trunk
plus commits that fix problems in the mcs51 port and in the code that mcs51 has in common with
other ports. Most of the problems are open tickets in SDCC's trackers.

The findings on each ticket are in [sdcc-triage](https://github.com/mdmower/sdcc-triage), a report
for SDCC's maintainers. The report has the commits of this fork as patches for trunk.

The fixes were written with an AI coding assistant (Claude), which the owner of this repository
directed. SDCC's maintainers have not reviewed them.

## Commits

Commit subjects start with the tracker and the ticket number, such as `[bugs #3903]`. Commits
marked `[local #NNNN]` fix problems that do not have a ticket.

## Branches and tags

`main` is the only branch. It is a linear series on top of trunk with one commit for each fix, so
that each commit is a patch for trunk.

`main` is rebased onto trunk each time the fork is synchronized with it. Merging trunk into `main`
would keep the commit hashes, but the fixes would then not be a series of patches for the current
trunk. A rebase gives every commit a new hash, so `main` is force pushed after each
synchronization. Between synchronizations, new fixes are added at the head and pushed as
fast-forwards.

To update a clone after a synchronization, run `git fetch` and `git reset --hard origin/main`. Keep
your own commits on another branch, because the reset discards them.

The tags `archive/rNNNNN` are permanent. Each one is the head of `main` before a synchronization,
where rNNNNN is the trunk revision that this head was based on. Use these tags in links that have
to stay valid.

## Building

Build and install the compiler as [doc/README.txt](doc/README.txt) and
[doc/INSTALL.txt](doc/INSTALL.txt) describe.
