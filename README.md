# hypr-autoscroll

Windows-style middle-click autoscrolling for Hyprland.

Enable middle-button autoscroll mode, middle-click a scrollable surface, then
move the pointer away from the activation point. Direction controls the scroll
direction and distance controls the speed.

The plugin sends native Wayland continuous-axis events from inside Hyprland. It
does not require root access, `/dev/input`, `uinput`, or a background input
daemon.

> [!WARNING]
> Hyprland plugins run inside the compositor and are ABI-sensitive. Install
> with `hyprpm` so the plugin is built against your Hyprland version. Do not
> copy a prebuilt `.so` from another system.

## Features

- Smooth vertical and horizontal autoscrolling
- Configurable sensitivity, acceleration, dead zone, and maximum speed
- Fixed scroll target while the pointer moves
- Native `all-scroll` cursor while scrolling
- Shortcut-controlled middle-button mode
- Normal middle-button behavior when the mode is disabled
- Automatic exit on another click or physical wheel input
- Pointer-constrained applications such as games are ignored
- Lua and legacy Hyprland configuration support

## Requirements

- Hyprland with plugin support
- `hyprpm`
- A compiler and Hyprland headers when building manually

Hyprland 0.55.2 and 0.56.0 are currently verified. Other releases may require
a matching plugin revision because Hyprland does not guarantee plugin ABI
compatibility.

## Install with hyprpm

```bash
hyprpm add https://github.com/estebanhiramramirezgomez/hypr-autoscroll
hyprpm enable hypr-autoscroll
hyprpm reload
```

To load enabled plugins when Hyprland starts, add the following to your Lua
autostart configuration:

```lua
hl.on("hyprland.start", function()
  hl.exec_cmd("hyprpm reload")
end)
```

For legacy configuration:

```ini
exec-once = hyprpm reload
```

## Recommended setup

The recommended setup keeps the middle button normal by default. `SUPER + A`
toggles whether middle-click activates autoscroll:

```lua
hl.config({
  plugin = {
    hypr_autoscroll = {
      direct_activation = false,
    },
  },
})

hl.bind("SUPER + A", function()
  if hl.plugin.hypr_autoscroll then
    hl.plugin.hypr_autoscroll.middle_mode("toggle")
  end
end, {
  description = "Toggle middle-button autoscroll",
})
```

Usage:

1. Press `SUPER + A` to enable middle-button autoscroll mode.
2. Middle-click a scrollable window.
3. Move away from the activation point to scroll.
4. Click or use the physical wheel to end the current scroll session.
5. Press `SUPER + A` again to restore normal middle-button behavior.

Hyprland displays a notification when the middle-button mode changes.

### Omarchy

Put the `hl.config` block in `~/.config/hypr/hyprland.lua` and the `hl.bind`
block in `~/.config/hypr/bindings.lua`. Keep customizations under
`~/.config/hypr/`; files under `~/.local/share/omarchy/` are managed by
Omarchy.

## Configuration

```lua
hl.config({
  plugin = {
    hypr_autoscroll = {
      enabled = true,
      direct_activation = false,
      button = 274,
      dead_zone = 12.0,
      sensitivity = 8.0,
      acceleration = 1.15,
      max_speed = 3000.0,
      horizontal = true,
      vertical = true,
      frame_interval_ms = 16,
    },
  },
})
```

| Option | Default | Description |
| --- | ---: | --- |
| `enabled` | `true` | Enables the plugin |
| `direct_activation` | `true` | Starts with middle-button autoscroll mode enabled |
| `button` | `274` | Linux input button code; `274` is `BTN_MIDDLE` |
| `dead_zone` | `12.0` | Pointer distance before scrolling starts |
| `sensitivity` | `8.0` | Scroll velocity multiplier |
| `acceleration` | `1.15` | Speed curve exponent |
| `max_speed` | `3000.0` | Maximum scroll velocity |
| `horizontal` | `true` | Enables horizontal scrolling |
| `vertical` | `true` | Enables vertical scrolling |
| `frame_interval_ms` | `16` | Delay between generated scroll frames |

Legacy configuration uses the same option names:

```ini
plugin {
    hypr_autoscroll {
        enabled = true
        direct_activation = false
        sensitivity = 8
    }
}

bind = SUPER, A, hypr-autoscroll:middle-mode, toggle
```

## Commands

### Middle-button mode

The Lua function accepts `toggle`, `on`, `off`, and `status`, and returns the
resulting mode state:

```lua
hl.plugin.hypr_autoscroll.middle_mode("toggle")
```

Legacy dispatcher:

```ini
hypr-autoscroll:middle-mode, toggle
```

### Current scroll session

The separate session command starts or stops autoscrolling immediately at the
current pointer:

```lua
hl.plugin.hypr_autoscroll.toggle("toggle")
```

Legacy dispatcher:

```ini
hypr-autoscroll:toggle, toggle
```

Both commands accept `toggle`, `on`, and `off`.

## Build from source

On Arch Linux:

```bash
sudo pacman -S --needed base-devel hyprland
make
make test
```

The plugin is written to `build/hypr-autoscroll.so`.

Load a local build:

```bash
hyprctl plugin load "$(pwd)/build/hypr-autoscroll.so"
```

Unload it:

```bash
hyprctl plugin unload "$(pwd)/build/hypr-autoscroll.so"
```

CMake is also supported:

```bash
cmake -S . -B build-cmake -DBUILD_TESTING=ON
cmake --build build-cmake
ctest --test-dir build-cmake --output-on-failure
```

## Behavior and limitations

- Autoscroll is refused when the session is locked, no pointer-focused surface
  exists, or a client has constrained the pointer.
- Pointer motion is withheld from clients during a scroll session so scrolling
  remains attached to the original surface.
- A non-activation click restores normal pointer focus before the click is
  passed through.
- There is not yet a fixed visual marker at the activation point.
- Applications can interpret continuous-axis values differently, so tuning
  may vary slightly between toolkits.

## Troubleshooting

After updating Hyprland, rebuild or update the plugin:

```bash
hyprpm update -f
hyprpm reload
```

List loaded plugins:

```bash
hyprctl plugin list
```

Check configuration errors:

```bash
hyprctl configerrors
```

If Hyprland rejects a manually built plugin, clean and rebuild it against the
currently installed headers:

```bash
make clean
make
```

## Contributing

Bug reports and pull requests are welcome. When reporting a problem, include:

- Hyprland version
- Application and toolkit, if known
- Relevant plugin configuration
- Steps that reproduce the behavior

Please run both build paths and the tests before submitting changes.

## License

BSD 3-Clause. See [LICENSE](LICENSE).
