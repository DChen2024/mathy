# Changelog

- [Version 1.1.0](#110-2025-01-01)
- [Version 1.0.0](#100-2024-11-01)

## 1.1.0 (2025-01-01)

This release redesigns some aspects of the library and includes breaking
changes.

### Added

- **Statistical analysis**: Introduce statistics analysis functions.

- **Examples**: Create examples to demonstrate usge of the library.

### Changed

- **Naming convention**: Append a prefix to all function names to avoid name
collision and renamed some functions.

- **Version encoding**: Replace version encoding as a constant with version
encoding as a preprocessor macro.

- **Documentation revisions**: Modify documentation and changelog formatting
and provide more detail on the properties of statistics functions.

- **Increased accuracy**: Enhance the accuracy of `mathy_stats_binom_pdf` and
`mathy_stats_binom_cdf` for large values of `n`.

### Fixed

- **Division by zero**: Resolve issue where `mathy_isqrt` and `mathy_icbrt`
could result in undefined behavior due to possible division by zero.

- **Output range**: Resolve issue where `mathy_stats_binom_cdf` could return
a value greater than 1 due to intermediate rounding.

### Removed

- **Inconsistent features**: Discontinue `mathy_prime` and `mathy_quadratic`
as they do not align with the overall design of the library.

## 1.0.0 (2024-11-01)

We are excited to announce the release of version 1.0.0! This marks the first
stable release of the library, featuring some basic math utility functions not
part of the C/C++ standard library. We appreciate your support and feedback as
we continue to improve the library in future updates.

### Added

- **Core functionalities**: Introduce basic features of the library.

- **Benchmarking**: Create tests to evaluate the performance of various
implementations for some functions.

- **Contributing guidelines and license**: Establish guidelines for
contributing to the project, along with license information.
