# Style Guide Plugin for Unreal Engine 5

This plugin attempts to let developers enforce customizable style guides (like the one from ue4.style), either through global developer settings, or on per-directory basis. 
It uses UE's data validation framework, so it will validate all assets on save (if data validation is turned on, read epic's documentation for more information)

## Installation

Drop the StyleGuidePlugin into %project_dir%/Plugins/ directory, ensure it's enabled in your project

## Uinstallation

Remove the plugin from %project_dir%/Plugins/

## Usage

This plugin allows for flexible style guide rules. Assets will try to match rules according to hierarchy, first will check for any global rulesets, then will recursively check any directories, including directory where an asset is placed in. 
More nested guide wins (if multiple Style Guide Setup assets exist in a directory, first of all, you're doing things incorrectly, but alas, will check alphabetically).

### Global 

Global settings are rulesets placed in Project Settings -> Editor -> Style Guide. They will be the first considered ruleset. Any following ruleset can override those settings. 
They require creating a Style Guide Setup asset, but will work regardless of whether or not it is contained in validated asset's path.

### Local Settings

You can create a Style Guide Setup asset in any directory, and those guidelines will automatically be applied to assets in this, and child directories.

### Style Guide Setup

This asset type is created by creating a data asset, and choosing "Style Guide Setup" class. It contains modules, which can turn on different validation (Filename validation, blueprint functions and variables identifiers checks, etc.)

## License

I have released this plugin on MIT license, which means you can fork this plugin, make changes and redistribute without restrictions. If you can, consider crediting, and placing a link to this repository in your project.
