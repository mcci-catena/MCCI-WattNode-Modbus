# MCCI-WattNode-Modbus

Header files and code for working with the Continental Controls WattNode&reg; Modbus energy meters.

<!-- markdownlint-disable MD033 MD004 -->
<!-- markdownlint-capture -->
<!-- markdownlint-disable -->
<!-- TOC depthFrom:2 updateOnSave:true -->

- [Overview](#overview)
    - [Header Files](#header-files)
        - [`MCCI-WattNode-Modbus-Registers.h`](#mcci-wattnode-modbus-registersh)
        - [`MCCI-WattNode-Modbus-tuple.h`](#mcci-wattnode-modbus-tupleh)
    - [Namespaces](#namespaces)
    - [Classes](#classes)
        - [`WattNodeModbus_Framework`](#wattnodemodbus_framework)
        - [`WattNode_WNC`](#wattnode_wnc)
        - [`WattNode_WND`](#wattnode_wnd)
    - [Registers](#registers)
    - [Static Methods](#static-methods)
        - [`getAddress()`](#getaddress)
        - [`getRegister()`](#getregister)
        - [`regval_getfloat()`](#regval_getfloat)
        - [`float_getregval()`](#float_getregval)
        - [`regval_getuint32()`](#regval_getuint32)
        - [`int32_getregval()`](#int32_getregval)
- [Supported meters](#supported-meters)
- [References](#references)
- [Procedure For Generating Spreadsheet](#procedure-for-generating-spreadsheet)
- [Acknowledgements](#acknowledgements)

<!-- /TOC -->
<!-- markdownlint-restore -->
<!-- Due to a bug in Markdown TOC, the table is formatted incorrectly if tab indentation is set other than 4. Due to another bug, this comment must be *after* the TOC entry. -->

## Overview

This library defines C++ classes that encapsulate the Modbus register definitions for the Continental Control Systems WattNode Modbus electric power meters. In addition to register definitions, utility functions are provided for converting between higher level data types and 16-bit Modbus register values.

### Header Files

#### `MCCI-WattNode-Modbus-Registers.h`

This header file defines the classes listed under [Classes](#classes), below.

#### `MCCI-WattNode-Modbus-tuple.h`

This auxiliary header file wraps the standard C++ header file `<tuple>`, compensating for some of the complications of the Arduino environment.

### Namespaces

All C++ names are declared in a single namespace `McciWattNodeModbus`. In the rest of this document, we assume that you have written:

```c++
using namespace McciWattNodeModbus;
```

Of course, this is not a requirement; you can use the namespace on each reference, at the cost of a lot more typing.

### Classes

#### `WattNodeModbus_Framework`

```c++
class WattNodeModbus_Framework;
```

This class defines the common [static methods](#static-methods) exported by the concrete `WattNodeModbus` classes.

#### `WattNode_WNC`

```c++
class WattNode_WNC : public WattNodeModbus_Framework;
```

This class extends `WattNodeModbus_Framework` by defining `enum Register`, which in turn defines the various register names for the WattNode WNC meter. See [Registers](#registers) for general considerations.

#### `WattNode_WND`

```c++
class WattNode_WND : public WattNodeModbus_Framework;
```

This class extends `WattNodeModbus_Framework` by defining `enum Register`, which in turn defines the various register names for the WattNode WND module. See [Registers](#registers) for general considerations.

### Registers

The names of registers follow a common format, <code><it>RegisterName</it>_<it>type</it></code>, where <code><it>RegisterName</it></code> matches the name used in the Continental Controls manual, and <code><it>type</it></code> is a reminder of the numeric type to be used for the register. The <code><it>type</it></code> is one of the following:

| <code><it>type</it></code> | Description |
|:--------------------------:|:------------|
| `i32`   | The value of the `enum` is the address of the first register of a pair of registers, which are to be combined and treated as a 32-bit twos-complement signed integer. The lower-numbered register is the low-order word.
| `f32`   | The value of the `enum` is the address of the first register of a pair of registers, which are to be combined and treated as a 32-bit IEEE floating point number. The lower-numbered register is the low-order word.
| `i16`   | The value of the `enum` is the address of a 16-bit register, which is to be treated as a 16-bit twos-complement signed integer.
| `u16`   | The value of the `enum` is the address of 16-bit register, which is to be treated as a 16-bit unsigned integer.
| `vu16`  | The value of the `enum` is the address of the first register in a vector of registers.

**_Note:_** all register values are origin-1, to match Continental Controls documentation and Modbus convention.

### Static Methods

#### `getAddress()`

```c++
constexpr uint16_t WattNodeModbus_WNx::getAddress(Register r);
```

This function converts a register value to an address suitable to passing over the wire, by converting the origin-1 `Register` value to origin-0.

#### `getRegister()`

```c++
constexpr Register WattNodeModbus_WNx::getRegister(uint16_t address);
```

This function converts a Modbus address into the the equivalent `Register` value. It doesn't check whether the address matches any known register, it just blindly does a type conversion.

#### `regval_getfloat()`

```c++
static float WattNodeModbus_WNx::regval_getfloat(int16_t vLow, int16_t vHigh);
static float WattNodeModbus_WNx::regval_getfloat(int16_t vLow, int16_t vHigh, bool *pError);
```

Given two values read from consecutive registers, create the equivalent IEEE floating point number. In order to avoid run-time exceptions, IEEE NAN values are converted to zero. If `pError` is supplied and is not `nullptr`, then `*pError` is set `true` if a NAN is encountered. Otherwise `*pError` is not changed.

#### `float_getregval()`

```c++
static std::tuple<int16_t, int16_t>
WattNodeModbus_WNx::float_getregval(
    float vFloat
    );
```

Converts a floating-point value to the equivalent pair of registers to be transmitted over the wire. The result is a `tuple` with the first word being the low-order register value, and the second word being the high-order register value. Here's an example.

```c++
int16_t vLow, vHigh;
float vFloat = // something;
std::tie(vLow, vHigh) = WattNodeModbus_WNx::float_getregval(vFloat);
```

#### `regval_getuint32()`

```c++
static int32_t
WattNodeModbus_WNx::regval_getint32(
    int16_t vLow, int16_t vHigh
    );
```

Given two 16-bit values read from consecutive registers, create the equivalent 32-bit integer.

#### `int32_getregval()`

```c++
static std::tuple<int16_t, int16_t>
WattNodeModbus_WNx::int32_getregval(
    int32_t vInt32
    );
```

Convert an `int32_t` value into the equivalent pair of 16-bit values to be placed into consecutive registers.  The result is a `tuple` with the first word being the low-order register value, and the second word being the high-order register value. Here's an example.

```c++
int16_t vLow, vHigh;
int32_t vInt32 = /* some value */;

std::tie(vLow, vHigh) = WattNodeModbus_WNx::int32_getregval(vInt32);
```

## Supported meters

We've used this library with the following meters.

* [WND-M1-MB](https://ctlsys.com/product/wattnode-module-modbus/)
* [WNC-3Y-208-MB](https://ctlsys.com/product/wattnode-modbus/) and other members of the WNC family.

## References

All this data is based on downloads from Continental Control Systems' website, [ctlsys.com](https://www.ctlsys.com).

The spreadsheets we used to help with this work are in [extras](./extras).

The original spreadsheet was the reference for WND series, the V1.8 [Register List](https://r1spn12mh523ib7ly1ip4xkn-wpengine.netdna-ssl.com/wp-content/uploads/2016/10/WNC-Modbus-Register-List-V18.xls)

This was updated in April 2019 based on the [WND-Module-Modubs-Register-List-v1028](https://r1spn12mh523ib7ly1ip4xkn-wpengine.netdna-ssl.com/wp-content/uploads/2017/10/WND-Module-Modbus-Register-List-v1028.xls). We took their word for the names of the new registers.

## Procedure For Generating Spreadsheet

Duplicate first sheet of spreadsheet.

Rename first sheet "Main List", and copy "Flattened".

From now own, all work is in the "Flattened" sheet.

Delete lines above the section title, e.g. "Basic Registers - Floating Point" or "Measurement Register List - Floating-Point"

Insert a new column at column A (so you have two blank columns on the left).

We're going to pivot the top-level headers into column A, and pivot level-two header into column B.  While doing that, normalize the titles so that we have "floating point" or "integer"  in the f32 or i32 registers.

It's fastest to pivot layer 2 first.

Then delete line 1, select all, turn on filtering, and use filtering to make it easier to find the header rows. Delete them.

Insert two columns to right of "`registers`". Copy "`Base (hex)`" and "`Base (dec, origin-1)`" from an earlier sheet. Formulas:

Column | Formula
-----|------
Base (hex) | `=DEC2HEX(NUMBERVALUE(LEFT(E2,IFERROR(SEARCH(",",E2)-1,LEN(E2))))-1)`
Base (dec) | `=HEX2DEC(F2)+1`
Size | `=IF(IFERROR(SEARCH(",",E2),0)<>0, 2, 1)`

Similarly, insert 4 columns to right of "Name", and copy/paste from an earlier sheet.  Formulae:

Column | Formula
-----|------
Size | `=IF(IFERROR(SEARCH(",",C2),0)<>0, 2, 1)`
Type | `=IF(IFERROR(SEARCH("floating point",A2),0)<>0, "float", IF(G2>1, "int32_t", "int16_t"))`
C++ name | `=CONCAT(F2,"_",LEFT(H2,1),16*G2)`
Enum | `=CONCAT(I2, IF(IFERROR(E2-E1<>1,TRUE),CONCAT(" = ",E2),CONCAT(" /* = ", E2, " */")),",")`

Fill down to fill the inserted columns with formula.

Sort ascending on "`Base (dec, origin-1)`".

## Acknowledgements

WattNode is a registered trademark of Continental Controls. MCCI and MCCI Catena are registered trademarks of MCCI Corporation. All other trademarks are the properties of their respective owners.
