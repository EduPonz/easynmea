# GNSS Interface Versioning Policy

*GNSS Interface* adheres to the [Semantic Versioning 2.0.0](https://semver.org/) versioning is
schema.
Following this convention, the library's version is comprised of three elements, a.k.a MAJOR version, MINOR version, and PATCH version.
The different parts of given version number `MAJOR.MINOR.PATCH` define:

1. **MAJOR**: API compatibility.
   All *GNSS Interface* versions with the same **MAJOR** component are API compatible.
   Furthermore, versions with different **MAJOR** component are NOT API compatible.
   It is important to note that removing existing features results in a **MAJOR** version change.
   Upgrading to a different *GNSS Interface* **MAJOR** version may require source code modification depending on the APIs used.
2. **MINOR**: ABI compatibility and feature coherence.
   All *GNSS Interface* versions with the same **MINOR** component are ABI compatible and have the same features.
   Furthermore, versions with different **MINOR** component are either not ABI compatible, or do not have the same features, or both at the same time.
   This means that both breaking ABI compatibility and adding new features must result in a change in the version's **MINOR** component.
   Upgrading to a different *GNSS Interface* **MINOR** version may require recompilation depending
   on the APIs used.
3. **PATCH**: Compatible bug fixes.
   *GNSS Interface* patch releases bundle several backwards compatible bug fixes together in a way that is both API and ABI compatible.
   Upgrading to a different *GNSS Interface* **PATCH** version does not require any source code modification, nor does it require recompiling.

```
MAJOR.MINOR.PATCH
  |     |     |
  |     |     +-- Compatible bug fixes
  |     +-------- ABI compatibility and feature coherence
  +-------------- API compatibility
```
