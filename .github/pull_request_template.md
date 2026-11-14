## What does this change?

<!-- A short summary. Link issues it closes, e.g. "Closes #12". -->

## Affects

- [ ] IR Sensor Ring firmware
- [ ] Laser Distance Sensor Array firmware
- [ ] Shared core (hub communication)
- [ ] Docs only
- [ ] Build / CI / tooling

## Does it change what teams see?

<!-- Any change to what the R/G/B/I channels report, the update rate or the special values (450, 9999, 65535) changes robots' behaviour. -->

- [ ] No: same channel values and behaviour
- [ ] Yes, and:
  - [ ] the changelog (`docs/changelog.md`) says what changed and what teams need to do
  - [ ] the product docs show it for the new firmware version, with the old version kept in its tab

## Testing

- [ ] CI is green (firmware build, Arduino Lint, unit tests)
- [ ] Unit tests added or updated if the maths changed (`make -C extras/test`)
- [ ] Tested on hardware: <!-- which product(s), how; or "not needed" and why -->

## Release (only for release PRs)

- [ ] Version bumped in **both** `library.properties` and `src/AdvancedBotWorksVersion.h`
