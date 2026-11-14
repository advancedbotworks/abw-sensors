<!--
  PRODUCT PAGE TEMPLATE (internal, not published on the docs site)

  How to use it:
  1. Copy this file to docs/products/<product>-v<N>.md, e.g. ir-ring-v2.md
  2. Add the page to the "Products" section of `nav` in mkdocs.yml
  3. Fill in every field. If a value isn't known yet, write "TBC" and add it
     to the "Still to confirm" list at the bottom, so nothing is forgotten.
  4. Delete these instructions and any sections that genuinely don't apply.

  Conventions:
  - Units: mm, g, V, mA, Hz, degrees. Give typical AND max where it matters.
  - "Measured" values must say how they were measured (setup, sample size).
  - "Rated" values come from component datasheets; say which part.
  - One page per hardware version. Never overwrite an old version's page:
    customers who own it still need it.
-->

# <Product name> (v<N>)

<One or two sentences: what it does and what it's for, e.g. which RoboCup Junior league.>

!!! info "At a glance"
    - **Measures:** <what>
    - **Connects to:** LEGO® SPIKE™ Prime hub, any port (shows up as a colour sensor)
    - **Firmware:** `<SketchName>` from the [AdvancedBotWorks library](../firmware/library.md), v<x.y.z> or later
    - **Shop:** [<product page>](https://advancedbotworks.com/products/...)

## Channel outputs

| Channel | Meaning | Range | Units | Notes |
|---|---|---|---|---|
| Red (R) | | | | |
| Green (G) | | | | |
| Blue (B) | | | | |
| (4th value) | unused | 0 | | |

## Quick start

1. Plug the sensor into any hub port.
2. <Product-specific step, e.g. orientation or which way is "forward".>
3. Read the R/G/B values (see [Getting started](../getting-started.md)).

## Example code

=== "Word Blocks"

    <Describe the blocks or add a screenshot.>

=== "SPIKE App 3 Python"

    ```python
    from hub import port
    import color_sensor

    r, g, b, i = color_sensor.rgbi(port.A)
    ```

## Specifications

### Physical

| | |
|---|---|
| Dimensions (L × W × H) | TBC mm |
| Weight | TBC g |
| Mounting | <LEGO stud/pin grid? hole spacing in studs/mm> |
| Materials / enclosure | TBC |
| Cable | <type, length, connector> |

### Electrical

| | |
|---|---|
| Supply | From the SPIKE hub port (<voltage>) |
| Current draw | typ. TBC mA / max TBC mA (measured) |
| Microcontroller | <part number> |
| Sensing parts | <part numbers and quantity> |

### Performance

| | |
|---|---|
| Update rate | TBC Hz (measured, fixed in firmware?) |
| Range | <rated (datasheet)> / <measured> |
| Accuracy / resolution | TBC |
| Field of view / angles | TBC |
| Latency | TBC ms |

### Operating conditions

These define the "quoted working conditions" referred to by the warranty.

| | |
|---|---|
| Temperature | TBC °C |
| Lighting / interference | <e.g. direct sunlight, other IR sources> |
| Handling | Not a toy. Ages 14+. |

## In the box

- 1 × <Product name> with attached cable

## Mounting

<Diagram or photo with the mounting points and the "forward" direction marked.>

## Firmware

- Ships with: `<SketchName>`, library v<x.y.z>
- Update guide: [Updating firmware](../firmware/updating.md)

## Known limitations

- <Honest list: conditions where readings degrade, etc.>

## Revision history

| Version | Date | Changes |
|---|---|---|
| v<N> | <YYYY-MM> | First release |

!!! warning "Still to confirm"
    - <Every TBC above, so they get filled in>
