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

`main` is rebased each time the fork is synchronized with trunk, so its commit hashes change. The
tags `archive/rNNNNN` keep the earlier heads. Each tag is named after the trunk revision that its
head was based on.

## Building

Build and install the compiler as [doc/README.txt](doc/README.txt) and
[doc/INSTALL.txt](doc/INSTALL.txt) describe.
