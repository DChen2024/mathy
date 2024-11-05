# Changelog

- [Version 1.0.1](#100-2024-xx-xx)
- [Version 1.0.0](#100-2024-11-01)

## 1.0.1 (2024-xx-xx)

This release includes minor bug fixes and improvements.

### Changed

- **Increased accuracy**: Enhanced the accuracy of `binom_pdf` and `binom_cdf`
for large values of `n`.

- **Documentation refinements**: Refined documentation for better clarity and
adjusted changelog formatting.

### Fixed

- **Division by zero**: Resolved issue where `isqrt` and `icbrt` could result
in undefined behavior due to possible division by zero.

- **Output range**: Resolved issue where `binom_cdf` could return a value
greater than 1 due to intermediate rounding.

## 1.0.0 (2024-11-01)

We are excited to announce the release of version 1.0.0! This marks the first
stable release of the library, featuring some basic math functions not part
of the C/C++ standard library. We appreciate your support and feedback as we
continue to improve the library in future updates.

### Added

- **Core functionalities**: Introduced basic features of the library.

- **Benchmarking**: Created tests to evaluate the performance between a pure
Python implementation and a C extension implementation

- **Contributing guidelines and license**: Established guidelines for
contributing to the project, along with license information.
