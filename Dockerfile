FROM debian:12-slim

USER root

RUN apt-get update && apt-get install --no-install-recommends -y git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu

WORKDIR /home/trukhinmaksim

EXPOSE 25000

CMD ["/bin/bash"]
