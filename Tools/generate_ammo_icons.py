#!/usr/bin/env python3
"""
Generate ammo icons from a bullet overlay and per-segment masks.

Expected inputs:
- base image: RGBA bullet overlay, typically black outline with transparent interior
- mask images: grayscale/RGBA masks for segment 1, 2 and 3

Each generated icon is built by:
1. filling each segment mask with a color
2. compositing the base overlay on top

Example:
python Tools/generate_ammo_icons.py ^
  --base Assets/AmmoIcons/bullet_outline.png ^
  --mask Assets/AmmoIcons/mask_1.png ^
  --mask Assets/AmmoIcons/mask_2.png ^
  --mask Assets/AmmoIcons/mask_3.png ^
  --output Assets/AmmoIcons/Generated
"""

from __future__ import annotations

import argparse
from itertools import product
from pathlib import Path
from typing import Iterable

from PIL import Image, ImageChops


# Keep this aligned with EShapeColor enum order.
DEFAULT_COLOR_ORDER: list[tuple[str, tuple[int, int, int, int]]] = [
    ("red", (255, 38, 38, 255)),
    ("blue", (38, 89, 255, 255)),
    ("green", (26, 217, 51, 255)),
    ("yellow", (255, 217, 26, 255)),
    ("gray", (128, 128, 128, 255)),
    ("wildcard", (242, 77, 255, 255)),
]

COLOR_INITIALS: dict[str, str] = {
    "red": "r",
    "blue": "b",
    "green": "g",
    "yellow": "y",
    "gray": "x",
    "wildcard": "w",
}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate ammo icons from color masks.")
    parser.add_argument("--base", required=True, type=Path, help="RGBA bullet overlay image.")
    parser.add_argument(
        "--mask",
        required=True,
        type=Path,
        action="append",
        help="Segment mask image. Pass this once per mask, in fill order.",
    )
    parser.add_argument("--output", required=True, type=Path, help="Output directory for generated PNG icons.")
    parser.add_argument(
        "--colors",
        nargs="*",
        default=[name for name, _ in DEFAULT_COLOR_ORDER if name != "gray"],
        help="Subset of color names to generate.",
    )
    parser.add_argument(
        "--flip-horizontal",
        action="store_true",
        help="Flip each generated icon horizontally before saving.",
    )
    return parser.parse_args()


def load_rgba_image(path: Path) -> Image.Image:
    return Image.open(path).convert("RGBA")


def load_mask_image(path: Path) -> Image.Image:
    image = Image.open(path)
    grayscale = image.convert("L")

    if "A" not in image.getbands():
        return grayscale

    alpha = image.getchannel("A")
    # Transparent pixels are always treated as black / not filled.
    return ImageChops.multiply(grayscale, alpha)


def resolve_color_palette(color_names: Iterable[str]) -> list[tuple[str, tuple[int, int, int, int]]]:
    palette_by_name = {name: value for name, value in DEFAULT_COLOR_ORDER}
    ordered_palette: list[tuple[str, tuple[int, int, int, int]]] = []

    for default_name, default_value in DEFAULT_COLOR_ORDER:
        if default_name in color_names:
            ordered_palette.append((default_name, default_value))

    missing = [name for name in color_names if name not in palette_by_name]
    if missing:
        raise ValueError(f"Unknown color names: {', '.join(missing)}")

    if not ordered_palette:
        raise ValueError("No valid colors selected.")

    return ordered_palette


def validate_sizes(base_image: Image.Image, masks: list[Image.Image]) -> None:
    for mask in masks:
        if mask.size != base_image.size:
            raise ValueError(
                f"Mask size {mask.size} does not match base image size {base_image.size}."
            )


def build_filename(slot_colors: tuple[str, ...]) -> str:
    initials = "".join(COLOR_INITIALS[color] for color in slot_colors)
    return f"{initials}.png"


def generate_icon(
    base_image: Image.Image,
    masks: list[Image.Image],
    slot_colors: tuple[str, ...],
    palette: dict[str, tuple[int, int, int, int]],
) -> Image.Image:
    filled = Image.new("RGBA", base_image.size, (0, 0, 0, 0))

    for mask, color_name in zip(masks, slot_colors, strict=True):
        color_layer = Image.new("RGBA", base_image.size, palette[color_name])
        filled = Image.composite(color_layer, filled, mask)

    return Image.alpha_composite(filled, base_image)


def main() -> int:
    args = parse_args()

    if len(args.mask) == 0:
        raise ValueError("At least one mask must be provided.")

    args.output.mkdir(parents=True, exist_ok=True)

    palette_entries = resolve_color_palette(args.colors)
    palette = {name: rgba for name, rgba in palette_entries}
    ordered_color_names = [name for name, _ in palette_entries]

    base_image = load_rgba_image(args.base)
    masks = [load_mask_image(mask_path) for mask_path in args.mask]
    validate_sizes(base_image, masks)

    for slot_colors in product(ordered_color_names, repeat=len(masks)):
        icon = generate_icon(base_image, masks, slot_colors, palette)
        filename_colors = slot_colors
        if args.flip_horizontal:
            icon = icon.transpose(Image.Transpose.FLIP_LEFT_RIGHT)
            filename_colors = tuple(reversed(slot_colors))
        output_path = args.output / build_filename(filename_colors)
        icon.save(output_path)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
