# Project Ignis card script check suite

This runs on Travis CI for our [card script collection](https://github.com/ProjectIgnis/CardScripts). Currently, it only performs a basic syntax check, but more features may be added in the future. 

This also serves as a demo for how to use the [redesigned EDOPro ocgcore C API](https://github.com/edo9300/ygopro-core). 

## Script syntax checker

Usage:
```
script_syntax_check [directories...]
```

All specified directories (default cwd if none provided) are searched for card scripts, including to one subdirectory level. The first script found with the name is always used, so do not specify multiple directories with scripts with the same name.

A basic Lua syntax check is done on scripts on pushes and pull requests. It loads `constant.lua` and `utility.lua` into ocgcore. Then it searches through one subfolder level for files of the form `cX.lua`, where `X` is an integer, loading them into the core as a dummy card with the same passcode. Three-digit passcodes and 151000000 (Action Duel script) are skipped as a workaround.

This catches basic Lua syntax errors like missing `end` statements and runtime errors in `initial_effect` (or a lack of `initial_effect` in a card script).

### Caveats

Does not catch runtime errors in other functions declared within a script unless they are called by `initial_effect` of some other script. This is not a static analyzer and it will not catch incorrect parameters for calls outside of `initial_effect` or any other runtime error.

Currently only works on POSIX due to assuming the POSIX filesystem API. Win32 support could be added trivially but hasn't been a priority. C++17 filesystem was not used because library support is missing from long-term release Linux distros used on Travis CI.

## Copyright notice and license

Copyright (C) 2020  Kevin Lu.
```
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
```
