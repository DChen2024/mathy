# Changelog

- [Version 1.1.1](#111-2025-xx-xx)
- [Version 1.1.0](#110-2025-01-01)
- [Version 1.0.1](#101-2024-11-05)
- [Version 1.0.0](#100-2024-11-01)

## 1.1.1 (2025-xx-xx)

This release includes minor bug fixes and improvements.

### Fixed

- **Compilation error**: Resolve issue where building the library would fail on
32-bit platforms and certain 64-bit platforms when using MSVC.

- **Inaccurate result**: Resolve issue where `mathy_isqrt` could return an
inaccurate value on platforms that do not support extended precision
floating-point.

## 1.1.0 (2025-01-01)

This release redesigns some aspects of the library and includes breaking
changes.

### Added

- **Statistical analysis**: Introduce a variety of statistics analysis
functions.

- **Examples**: Create examples to demonstrate usage of the library.

### Changed

- **Naming convention**: Append a prefix to all function names to avoid name
collision and renamed some functions.

- **Version encoding**: Replace version encoding as a constant with version
encoding as preprocessor macros.

- **Documentation revisions**: Modify changelog formatting and documentation
to provide additional details.

### Removed

- **Inconsistent features**: Discontinue `mathy_prime` and `mathy_quadratic`
as they do not align with the overall design of the library.

## 1.0.1 (2024-11-05)

This release includes minor bug fixes and improvements.

### Changed

- **Edge case handling**: Rework `binom_pdf` and `binom_cdf` to correctly
handle large values of `n`.

### Fixed

- **Undefined behavior**: Resolve issue where `isqrt` and `icbrt` could result
in undefined behavior due to possible division by zero.

- **Invalid result**: Resolve issue where `binom_cdf` could return a value
greater than 1 due to intermediate rounding.

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
