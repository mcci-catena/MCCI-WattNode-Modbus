# MCCI-WattNode-Modbus

Header files and code for working with the Continental Controls WattNode&reg; Modbus energy meter.

## Supported meters

We've used these files with the following meters.

* [WND-M1-MB](https://ctlsys.com/product/wattnode-module-modbus/)
* [WNC-3Y-208-MB](https://ctlsys.com/product/wattnode-modbus/) and other members of the WNC family.

## References

All this data is based on downloads from Continental Control Systems' website, [ctlsys.com](https://www.ctlsys.com).

The spreadsheets we used to help with this work are in [extras](./extras).

The original spreadsheet was the reference for WND series, the V1.8 [Register List](https://r1spn12mh523ib7ly1ip4xkn-wpengine.netdna-ssl.com/wp-content/uploads/2016/10/WNC-Modbus-Register-List-V18.xls)

This was updated in April 2019 based on the [WND-Module-Modubs-Register-List-v1028](https://r1spn12mh523ib7ly1ip4xkn-wpengine.netdna-ssl.com/wp-content/uploads/2017/10/WND-Module-Modbus-Register-List-v1028.xls). We took their word for the names of the new registers.

## Procedure

Duplicate first sheet of spreadsheet.

Rename first sheet "Main List", and copy "Flattened".

From now own, all work is in the "Flattened" sheeet.

Delete lines above the section title, e.g. "Basic Registers - Floating Point" or "Measurement Register List - Floating-Point"

Insert a new column at column A (so you have two blank columns on the left).

We're going to pivot th top-level headers into column A, and pivot level-two header into column B.  While doing that, normalize the titles so that we have "floating point" or "integer"  in the f32 or i32 registers.

It's fastest to pivot layer 2 first.

Then delete line 1, select all, turn on filtering, and use filtering to make it easier to find the header rows. Delete them.

Insert two columns to right of "registers". Copy "Base (hex)" and "Base (dec, origin-1)" from an earlier sheet. Formulas:

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

Sort ascending on "Base (dec, origin-1).

## Acknowledgements

WattNode is a registered trademark of Continental Controls. MCCI and MCCI Catena are registered trademarks of MCCI Corporation. All other trademarks are the properties of their respective owners.