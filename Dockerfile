FROM ghcr.io/wiiu-env/devkitppc:20260225

COPY --from=ghcr.io/wiiu-env/libmocha:20260126 /artifacts $DEVKITPRO

RUN git config --global --add safe.directory /project

WORKDIR /project