### 使用 Boost DLL
使用boost dll动态加载动态链接库

相比在Windows下直接LoadLibrary，使用boost.dll可以跨平台；动态加载动态链接库有很多好处，例如按需加载、热更新等等；

```my_plugin_api```演示了如何导出实例

```plugin_calculator```演示了如何导出工厂方法用于产生实例


