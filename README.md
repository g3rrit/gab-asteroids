# GBA Asteroids

## Building

This build requires [butano](https://github.com/GValiente/butano) as well as [devkitpro](https://devkitpro.org/wiki/Getting_Started).

On mac devkitpro can be installed via the `pkg` distribution:

```sh
sudo installer -pkg /path/to/devkitpro-pacman-installer.pkg -target /opt/devkitpro
```

Afterwards `devkitARM` can be installed:

```sh
pacman -S gba-dev
```

Butano can simply be cloned. It contains a template project in the `template` directory.

Additionally the following environment variables should be set:

```sh
export DEVKITPRO="/opt/devkitpro"
export DEVKITARM="/opt/devkitpro/devkitARM"
export LIBBUTANOABS="~/opt/butano/butano"
```
