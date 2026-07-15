### Checks

- **Please check and remove this text:**
  - Searched the [merge requests](https://gitlab.com/openfoam/core/openfoam/-/merge_requests) for duplicates.
  - Used the code guidelines in [CONTRIBUTING.md](./../../CONTRIBUTING.md).
  - Removed any irrelevant template sections.
  - Cleaned up commit history (no temporary files/WIP commits).
  - Used [GitLab Flavored Markdown](https://docs.gitlab.com/user/markdown/).
  - Used the `Preview` button to check formatting.

### Summary

<!--
One–two sentences to explain the contribution, e.g.

"Introduction of the dynamic-mesh capabilities into pisoFoam solver."
-->

**Contribution type (please check all that apply):**

- [ ] Bug fix
- [ ] New feature / functionality
- [ ] API change
- [ ] Clean‑up without any functional change
- [ ] Performance improvement
- [ ] Documentation
- [ ] Tutorial
- [ ] Non-bug behavioural change
- [ ] Compilation / installation
- [ ] Other (describe):

**Related issues:** <!-- e.g. "Closes #1234", "Relates to #5678" -->

### Description

**Motivation:**

<!--
- Problem statement: What specific problem does the feature solve? Why this contribution?
- Target: Which engineering field benefits most from this change?
-->

**Contribution:**

<!--
- Change: What is added/changed?
- Limitations: What are the problems/disadvantages of the contribution? Any existing alternatives?
-->

### Verification & Tests

**Environment:**

- HEAD base commit : <!-- e.g. `a1a3daca7e` -->
- Operating system : <!-- e.g. openSUSE Leap 15.5, Ubuntu 24.04, RHEL 9 etc. -->
- Compiler         : <!-- e.g. GCC 13.2.0, Clang 15.0.7, ICC, etc. -->
- MPI              : <!-- e.g. OpenMPI 4.1.6, MPICH 4.2.1, system vendor MPI etc. -->
- Options          : <!-- e.g. `linux64GccDPInt32Opt`, custom wmake rules etc. -->

**Minimal example case:**

<!--
Provide a minimal, reproducible test case (attachment or URL)
with precise, copy-pasteable steps or a ready-to-run script.

Preferably include parallel execution, interim write/read, and restart
actions in the script.

Exclude large or irrelevant data (e.g., post-processing, hidden files).
-->

**Backward compatibility:**

Select all that apply if the backward compatibility is intact:

- [ ] Public dictionaries and entries <!-- Can existing dictionaries run unmodified? Do existing dictionaries silently break?-->
- [ ] Public API <!-- Do existing core and community codes continue to compile unmodified without new warnings? -->
- [ ] Simulation output <!-- Do the new results match with the existing results within acceptable error bands, unless the changes are justified? -->
- [ ] Simulation performance <!-- Does the new performance match with the existing performance within acceptable error bands, unless the changes are justified? -->
- [ ] No new compilation warnings

<!--
Otherwise, if any changes occur, please quantify and explain the changes and/or any consequences.

Description of the new and/or improved changes to the public interface.

Any breaking changes, deprecation, removal, renaming?
-->

**Results:**

<!-- Illustration/description of the functional and performance tests and their results -->

<!----------------------------------------------------------------------------->
