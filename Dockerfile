FROM ghcr.io/wiiu-env/devkitppc:20250608

COPY --from=ghcr.io/wiiu-env/libmocha:20230621 /artifacts $DEVKITPRO

RUN git config --global --add safe.directory /project

WORKDIR /project