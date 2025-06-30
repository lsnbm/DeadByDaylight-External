# Dead by Daylight External Overlay  |  Language: [English](READMEEnglish.md)


> 基于内核驱动的物理内存直接读写，外部绘制工具
> **本项目不包含内核驱动程序**，仅提供外部绘图部分。

---

## 🔍 项目简介

Dead by Daylight External Overlay 是一款针对《黎明杀机》（Dead by Daylight）的外部绘制工具，
通过自研内核驱动对游戏物理内存进行读写，以获取游戏内实体信息并渲染到屏幕。
渲染部分基于 [noahware/external-overlay](https://github.com/noahware/external-overlay) 框架，
偏移量信息保持最新请参考 [Encryqed/Dumper-7](https://github.com/Encryqed/Dumper-7)。

---

## ⚙️ 功能列表

| 功能    | 描述               |
| ----- | ---------------- |
| 对局先知  | 实时显示当前游戏状态及玩家信息  |
| 显示技能  | 显示幸存者／杀手所携带的技能图标 |
| 显示杀手  | 高亮并跟踪杀手位置        |
| 显示位置  | 在地图上标注所有幸存者、杀手位置 |
| 显示距离  | 计算并显示与你的距离（米）    |
| 显示地窖  | 标记地窖（逃生口）位置      |
| 显示图腾  | 标注游戏中的图腾对象       |
| 显示木板  | 标记可互动的木板位置       |
| 显示发电机 | 标注并跟踪发电机进度及位置    |

---

## 🚀 快速开始

1. **下载本项目**

   ```bash
   git clone https://github.com/YourUser/DbD-External-Overlay.git
   cd DbD-External-Overlay
   ```

2. **准备内核驱动**
   ※ 本项目仅提供外部绘制代码，不包含驱动。
   请自行搭建或使用以下驱动模板，并确保已正确签名和加载：

   * 驱动示例：`YourKernelDriver.sys`
   * 驱动接口协议：自定义 `ReadPhysicalMemory` / `WritePhysicalMemory`

3. **配置偏移**
   偏移量请始终保持最新，下载并解析：

   ```bash
   git clone https://github.com/Encryqed/Dumper-7.git
   # 在 Dumper-7 工具中生成最新 offsets.json
   cp Dumper-7/offsets.json config/offsets.json
   ```

---


## 💡 使用说明

1. **启动游戏**《黎明杀机》
2. **加载内核驱动**（参考驱动文档）
3. **启动 External Overlay 程序**
4. INSERT隐藏菜单
5.无效请初始化或重启

---

## ⚖️ 免责声明

* 本项目仅供学习研究使用，请勿用于任何商业或非法用途。
* 使用本项目可能违反游戏服务条款，后果自负。
* 请谨慎评估并承担相应风险。

---

## ❤️ 致谢

* [noahware/external-overlay](https://github.com/noahware/external-overlay) —— 绘图框架
* [Encryqed/Dumper-7](https://github.com/Encryqed/Dumper-7) —— dump工具

