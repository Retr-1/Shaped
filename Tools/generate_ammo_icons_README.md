# Ammo Icon Generator

This script generates canonical bullet icons from:

- one RGBA base image
- one mask image per color slot

## Expected assets

- `base`
  - black bullet outline / overlay
  - transparent interior
  - same size as all masks
- `mask_1`, `mask_2`, `mask_3`
  - grayscale or RGBA mask images
  - white = filled area for that slot
  - same size as base

The script fills the masks in order, then composites the base image on top.

## Generation rules

Icons are generated for all ordered slot permutations using the selected colors.

`Gray` is excluded from generation because it represents junk shapes and should not produce bullet icons.

## Example

```powershell
python Tools/generate_ammo_icons.py `
  --base Assets/AmmoIcons/bullet_outline.png `
  --mask Assets/AmmoIcons/mask_1.png `
  --mask Assets/AmmoIcons/mask_2.png `
  --mask Assets/AmmoIcons/mask_3.png `
  --output Assets/AmmoIcons/Generated

To mirror the results horizontally:

```powershell
python Tools/generate_ammo_icons.py `
  --base Assets/AmmoIcons/bullet_outline.png `
  --mask Assets/AmmoIcons/mask_1.png `
  --mask Assets/AmmoIcons/mask_2.png `
  --mask Assets/AmmoIcons/mask_3.png `
  --output Assets/AmmoIcons/Generated `
  --flip-horizontal
```

When `--flip-horizontal` is used, the filename initials are reversed as well, so the name still matches the visible left-to-right slot order.
```

## Output naming

Files are written using slot initials in slot order:

- `bbr.png`
- `brb.png`
- `ggg.png`
- `ryw.png`

Initials:

- `r` = red
- `b` = blue
- `g` = green
- `y` = yellow
- `w` = wildcard
