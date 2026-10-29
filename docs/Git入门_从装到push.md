# Git 入门：从装到第一次 push

> 目标：20 分钟内，让你的练习代码出现在 GitHub 上。
> 只用 5 条命令，其他一律先不管。

---

## 第 1 步：装 Git（3 分钟）

**方式 A：一条命令（推荐）**

打开 PowerShell（开始菜单搜 `PowerShell`），粘贴：

```powershell
winget install --id Git.Git -e --source winget
```

看到 `已成功安装` 就成了。**然后关掉 PowerShell，重新开一个**（让 PATH 生效）。

**方式 B：官网下载**

https://git-scm.com/download/win → 下载 64-bit 安装包 → 一路 Next 默认即可。

安装时如果有一步叫 **"Choosing the default editor used by Git"**，选 **Use Visual Studio Code**。

### 验证

重开 PowerShell，输入：

```powershell
git --version
```

看到 `git version 2.xx.x` 就成功了。

---

## 第 2 步：配置身份（1 分钟，只做一次）

Git 每次提交都要记录"是谁改的"，所以要先告诉它你是谁。

```powershell
git config --global user.name "Hey103d"
git config --global user.email "你的GitHub注册邮箱"
```

> 邮箱必须和你 GitHub 账号一致，否则 GitHub 不会把你的提交算在你头上（绿格子不亮）。
> 忘了注册邮箱？登录 GitHub → 右上角头像 → Settings → Emails 就能看到。

顺便设置默认分支名为 `main`（GitHub 现在默认用这个）：

```powershell
git config --global init.defaultBranch main
```

---

## 第 3 步：把练习目录变成仓库（2 分钟）

```powershell
cd "E:\VS Code\cpp practice"
git init
```

看到 `Initialized empty Git repository` 就成了。

新建一个 `.gitignore`，告诉 Git 哪些文件**不要**上传（编译出来的 exe 又大又没用）：

```powershell
Set-Content .gitignore "output/`n*.exe`n.vscode/"
```

或者直接在 VSCode 里新建文件 `.gitignore`，写上三行：

```
output/
*.exe
.vscode/
```

---

## 第 4 步：在 GitHub 建一个空仓库（3 分钟）

1. 打开 https://github.com/new
2. **Repository name** 填 `cpp-practice`
3. 选 **Public**（私有也行，但 Public 才能当成简历用）
4. **下面三个勾选框（Add README / .gitignore / license）全部不要勾**
5. 点 **Create repository**

建完之后页面会给你一串命令，**不要照抄**，只记下这个名字：

```
https://github.com/Hey103d/cpp-practice.git
```

---

## 第 5 步：连起来，推上去（3 分钟）

```powershell
cd "E:\VS Code\cpp practice"
git remote add origin https://github.com/Hey103d/cpp-practice.git
git add .
git commit -m "day01-day04: 函数/指针/字符串/结构体/结构体数组"
git push -u origin main
```

**第一次 push 会弹出一个浏览器窗口让你登录 GitHub**，登录并点 Authorize 就行。
（这是 Git Credential Manager 在帮你保存凭据，以后不用再登。）

### 如果报 `src refspec main does not match any`

说明你本地分支叫 `master`，先改个名再推：

```powershell
git branch -M main
git push -u origin main
```

### 如果报 `Authentication failed`

在 Windows 搜索栏搜 **凭据管理器** → Windows 凭据 → 找到 `git:https://github.com` → 删除 →
再 push 一次，会重新弹浏览器。

---

## 第 6 步：以后每天只要 3 条

```powershell
cd "E:\VS Code\cpp practice"
git add .
git commit -m "day05: vector 容器练习"
git push
```

---

## 五个命令的心智模型

把 Git 想成**寄快递**：

| 命令 | 快递类比 | 实际作用 |
|---|---|---|
| `git status` | **看现在手里有什么** | 显示哪些文件改了、哪些已装箱。最常用，心里没底就敲它 |
| `git add .` | 把东西**放进箱子** | 把当前改动加入"待提交"清单（暂存区） |
| `git commit -m "..."` | **封箱贴标签** | 把这批改动打包成一个版本，写上一句话说明 |
| `git push` | **寄出去** | 把本地版本上传到 GitHub |
| `git log --oneline` | **查寄件记录** | 看历史提交列表 |

**只有 `git push` 才需要网。** 前三条都是在你电脑本地完成的。

---

## 不想敲命令？用 VSCode 点也行

装好 git 之后，VSCode 左侧会多出一个**源代码管理**图标（三个点连成树的样子，快捷键 `Ctrl+Shift+G`）：

1. 改完代码 → 面板里列出所有改动的文件
2. 在输入框写提交说明 → 点 **✓ 提交**
3. 点 **同步更改**（或右上角 `...` → 推送）

**本质完全一样**，只是把三条命令变成三次点击。

> 建议：**先敲命令学会，再用图形界面偷懒。** 因为命令你能看懂报错，按钮不能。

---

## 关于 `-m` 后面写什么

提交说明是写给三个月后的自己看的。两种写法对比：

```powershell
git commit -m "update"                          # 三个月后你完全不知道改了什么
git commit -m "day05: vector 替代裸数组，26/26"  # 一眼看懂
```

建议格式：`dayXX: 今天学了什么 + 结果`

---

## 一句话总结

> **git 不是备份工具，是"时间机器"。**
> 它让你在任何一个提交点都能回到过去、看到当时的完整代码。
> 你毕设写到一半发现今天全写错了，`git` 能救你——这才是它值钱的地方。
