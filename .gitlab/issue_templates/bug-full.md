### Checks

- **Please check and remove this text:**
  - This is not a usage/support/consultancy question.
  - Searched the [issue tracker](https://gitlab.com/openfoam/core/openfoam/-/issues) for duplicates.
  - Removed any irrelevant template sections.
  - Used [GitLab Flavored Markdown](https://docs.gitlab.com/user/markdown/).
  - Used the `Preview` button to check formatting.

### Summary

<!--
One–two sentences to explain the issue, e.g.

"pimpleFoam crashes with SIGFPE when using PIMPLE + AMI + LTS on v2512."
-->

**Reproducibility:** <!-- e.g., "always", "intermittent", "unknown", etc. -->

**Bug type (please check all that apply):**

- [ ] Run-time crash / hanging (SIGSEGV / SIGFPE / abort)
- [ ] Compilation failure
- [ ] Wrong / non-physical results
- [ ] Wrong theoretical approach
- [ ] Performance / scaling issue
- [ ] Documentation issue
- [ ] Tutorial issue
- [ ] Other (describe):

### Description

<!--

Please answer whichever apply:

- What is the problem? Expected and actual behaviours?
- What is not the problem?
- Where does the problem occur?
- Where does the problem not occur?
- When did the problem first occur?
- When was everything OK?
- What is the extent of the problem?
- Attach relevant logs/images/data. No full logs/data

-->

**Regression:** <!-- Did this work in a previous version? (e.g., v2406 OK, v2512 fails) -->

**Minimal example case:**

<!--
Provide a minimal, reproducible test case (attachment or URL).

Exclude large or irrelevant data (e.g., post-processing, hidden files).
-->

**Steps to reproduce:**

<!--
Provide precise, copy-pasteable steps or a ready-to-run script.

Example:

1. `cd $FOAM_RUN`
2. `cp -r $FOAM_TUTORIALS/incompressible/simpleFoam/pitzDaily .`
3. Modify `system/fvOptions` as follows: ...
4. `simpleFoam`
-->

### Environment

<!-- Remove any row below if not applicable -->

- OpenFOAM&reg; version : <!-- e.g. v2512, v2406, develop (SHA) etc. -->
- HEAD commit      : <!-- e.g. `a1a3daca7e` -->
- Installation     : <!-- e.g. Docker image, source build, operating-system build -->
- Operating system : <!-- e.g. openSUSE Leap 15.5, Ubuntu 24.04, RHEL 9 etc. -->
- Compiler         : <!-- e.g. GCC 13.2.0, Clang 15.0.7, ICC, etc. -->
- MPI              : <!-- e.g. OpenMPI 4.1.6, MPICH 4.2.1, system vendor MPI etc. -->
- Options          : <!-- e.g. `linux64GccDPInt32Opt`, custom wmake rules etc. -->
- Hardware info    : <!-- e.g. CPU model, RAM etc. -->
- Batch system     : <!-- e.g. SLURM etc. -->

### Possible solutions

<!--
- Code fixes or solution ideas or workaround ideas.
- Existing patches or MRs from external sources.
-->

<!----------------------------------------------------------------------------->
