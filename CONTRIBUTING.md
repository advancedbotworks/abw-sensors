# Contributing

Thanks for helping improve the Advanced Bot Works sensors!

Everyone taking part is expected to follow our [Code of Conduct](CODE_OF_CONDUCT.md). Many of our users are school students, so please keep things friendly and respectful.

## Reporting problems and ideas

- **Bugs and feature ideas:** [open an issue](https://github.com/advancedbotworks/abw-sensors/issues/new/choose). The forms ask for the product, firmware version and the values you're seeing.
- **Orders, shipping, returns or warranty:** email **support@advancedbotworks.com**, not GitHub. Please don't post order details publicly.
- **Setup and usage help:** start with the docs at **https://docs.advancedbotworks.com**, then ask in [Discussions (Q&A)](https://github.com/advancedbotworks/abw-sensors/discussions/categories/q-a) if you're still stuck.

## Changing the code

1. Fork the repository and create a branch.
2. Make your change. Follow the existing style:
    - classes PascalCase, files named after their class
    - methods camelCase, private members `m_camelCase`
    - public macros prefixed `ABW_`
    - Australian spelling ("Colour") in ABW code; third-party code keeps its original names
3. Format your code with clang-format 18 (`pip install clang-format==18.1.8`), using the style in `.clang-format`:
   ```sh
   make -C extras/test format
   ```
   CI checks formatting on every pull request.
4. If you change the IR Ring maths (`src/sensors/IRRingMath.*`), add or update tests and run them:
   ```sh
   make -C extras/test
   ```
5. Open a pull request. The template has a short checklist. CI compiles both firmware sketches for `arduino:samd:nano_33_iot`, runs Arduino Lint and the unit tests.

Changes that alter what the sensors report to the hub (the R/G/B/I values, rates or special values) affect every team using them. Please explain why the change is needed and update the changelog and docs.

## Licence

This project is licensed under the [GNU General Public License v3.0 or later](LICENSE). By contributing, you agree that your contribution is licensed under the same terms. Third-party code is listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
