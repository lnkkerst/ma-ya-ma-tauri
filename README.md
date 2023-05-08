# ma-ya-ma

C++ 大作业

Github Repo: [lnkkerst/ma-ya-ma-tauri](https://github.com/lnkkerst/ma-ya-ma-tauri)

## Release

可在 [release](https://github.com/lnkkerst/ma-ya-ma-tauri/releases) 页面找到已经构建出来的版本。

## 构建

请先确保满足[此处](https://tauri.app/v1/guides/getting-started/prerequisites)的要求。

以及安装好 [pnpm](https://pnpm.io/installation)。

```bash
# 克隆代码
git clone https://github.com/lnkkerst/ma-ya-ma-tauri.git && cd ma-ya-ma-tauri
# 安装依赖
pnpm i
```

### 使用 CMake

需要事先安装好 CMake。

还有 cxxbridge。

```bash
cargo install --force cxxbridge-cli
```

仅在 Linux(Arch distro) 上测试过，其它平台大概率失败。

构建：

```bash
mkdir build-release && cd build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

生成的产物（可执行二进制文件）为 `ma-ya-ma[.exe]`。

### 使用 Cargo

Windows 10, Linux(Ubuntu 18.04, Arch), macOS(M1, amd64) 下测试成功。

```bash
pnpm run build
```

生成的产物在 `src-tauri/target` 目录下。
