## 环境 / Environment 
- C++ 20
- Qt 6.9.1 (MSVC)
## IDE 
- CLion 2024.3.3 
## 默认配置 / Default Config 
- 二进制文件存储位置为 ``QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)`` ( 在 ``main.cpp`` Line 27 中定义 ) ( e.g. 在 Windows 系统中存储位置为 ``C:\Users\你的用户名\AppData\Roaming\LMS\`` ) 
- 在没有找到数据文件夹时，会创建新的数据文件，默认生成一个管理员用户， ID 为 ``100``，密码为 ``P@ssw0rd`` ( 在 ``User.cpp`` Line 246 中定义 ) 
## 扩展性 / Scalability 
#### 代码仅实现基础的图书、用户的增删查改功能，以及简单的借书、还书功能。在借鉴代码时，可以从以下几方面进行扩展： 
- i18n 国际化支持 ( 即多语言 ) 
- 批量操作数据 ( 批量添加、删除图书、用户 ) 
- 从文件导入 / 导出到文件 ( 可视化数据文件，如 ``.csv`` 、 ``.json`` ) 
- 数据流加密 
- 使用自定义主题 
## 说明 / Notice 
- 代码已经开源，可用于参考学习，应付 C++ 寒假作业 / 校企合作项目实战，但请不要照搬，避免查重。代码中一些难以理解的地方标有注释，但如果需要答辩，最好洞悉整个项目的作用。
- 如果是用于应付作业，请删除 LICENSE 文件。亦或是你想像我一样不愿意被学校索要代码版权，可以 fork 此项目，在不完全照搬源代码的情况下上传新的 LICENSE 文件，并开源在 GitHub 。
- 欢迎在 issue 中提出相关问题！
