/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "fpga/api/fpga_api.h"

namespace paddle {
namespace mobile {
namespace fpga {
namespace api {

static int fd = -1;
static const char *device_path = "/dev/fpgadrv0";

static inline int do_ioctl(int req, void *arg) { return ioctl(req, arg); }

int open_device() {
  if (fd == -1) {
    fd = open(device_path, O_RDWR);
  }
  return fd;
}

// memory management;
void *fpga_malloc(size_t size) {
  return reinterpret_cast<(void *)> mmap64(NULL, size, PROT_READ | PROT_WRITE,
                                           MAP_SHARED, fd, 0);
}

void fpga_free(void *ptr) { munmap(ptr, 0); }

void fpga_copy(void *dest, const void *src, size_t num) {
  memcpy(dest, src, num);
}

int ComputeFpgaConv(struct FpgaConvArgs) {}
int ComputeFpgaPool(struct FpgaPoolArgs) {}
int ComputeFpgaEWAdd(struct FpgaEWAddArgs) {}

}  // namespace api
}  // namespace fpga
}  // namespace mobile
}  // namespace paddle
