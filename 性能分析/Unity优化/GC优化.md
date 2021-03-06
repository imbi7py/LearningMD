[TOC]

#### 介绍：

　　在游戏运行的时候，数据主要存储在内存中，当游戏的数据在不需要的时候，存储当前数据的内存就可以被回收以再次使用。内存垃圾是指当前废弃数据所占用的内存，垃圾回收（GC）是指将废弃的内存重新回收再次使用的过程。

　　Unity中将垃圾回收当作内存管理的一部分，如果游戏中废弃数据占用内存较大，则游戏的性能会受到极大影响，此时垃圾回收会成为游戏性能的一大障碍点。

#### GC问题诊断

GC引起的性能问题可表现为帧率过低，帧率剧烈波动或者间歇性卡顿。但是其他问题也可能引起类似的症状。如果你的游戏有这些性能问题，首先需要使用Unity的Profiler工具来确定这些问题是由GC引起的。

如何使用Profiler工具来确定引起性能问题的原因，可以查看[这篇教程](https://unity3d.com/learn/tutorials/topics/performance-optimization/diagnosing-performance-problems-using-profiler?playlist=44069)。

#### Unity内存管理简介

基本上来说，Unity自动内存管理像这样工作：

- Unity可以访问两个内存池：**栈**和**堆**（也称为**托管堆**）。栈用于短期存储小块数据，堆用于长期存储和较大数据段。
- 当创建变量时，Unity从栈或堆中申请内存
- 只要变量在[作用域](https://unity3d.com/learn/tutorials/topics/scripting/scope-and-access-modifiers)内（仍然可以通过我们的代码访问），分配给它的内存仍然在使用中， 我们称这部分内存已被**分配**。 我们将栈中的变量称为**栈对象**，将堆中的变量称为**堆对象**。
- 当变量超出作用域，该内存不再被使用并可以归还给原来的内存池。当内存被归还给原有的内存池里，我们称该内存被**释放**。栈内存在变量超出作用域时被实时释放，而堆内存在变量超出作用域之后并没有被释放并保持被分配的状态
- **垃圾收集器**（garbage collector）识别和释放未使用的堆内存。 垃圾收集器定期运行以清理堆。

#### 在栈分配和释放时发生了什么

栈的分配和释放简单快速。这是因为栈只用于在短时间内存储小数据。 分配和释放总是以可预测的顺序发生，并且具有可预测的大小。

栈的工作方式类似于[栈数据类型](https://en.wikipedia.org/wiki/Stack_(abstract_data_type))：它是一个简单的元素集合，这种情况下的内存块，只能以严格的顺序添加和删除元素。 这种简单性和严格性使得它变得非常快速：当一个变量存储在栈上时，它的内存就是简单地从栈顶分配。 栈变量超出作用域时，用于存储该变量的内存将立即返回栈进行重用。

#### 在堆分配时发生了什么

堆分配比栈分配复杂的多。因为堆可以用来存储长期和短期数据及各种不同类型大小的数据。分配和释放也并不总是按可预测的顺序进行且可能需要大小差距巨大的内存块。

当一个堆变量创建时，将执行以下步骤：

- 首先，Unity检查堆上是否有足够的空闲内存，如果有，则该变量的内存被分配。
- 如果没有，Unity触发GC试图释放未使用的堆内存，这个操作可能很慢。如果GC之后堆内存足够，则该变量的内存被分配。
- 如果GC之后堆上还是没有足够的空闲内存，Unity将向操作系统申请更多内存以扩大堆大小。这个操作可能很慢。之后该变量的内存被分配。

堆分配可能会很慢，特别在必须执行GC和扩大堆大小时。

#### 在GC时发生了什么

当堆变量超出作用域后，存储该变量的内存并没有被立即释放。**无用的堆内存只在执行GC时被释放**。

每次执行GC时，将执行以下步骤：

- 垃圾收集器检索堆上的每个对象。
- 垃圾收集器搜索所有当前对象引用以确定堆上的对象是否仍在作用域内。
- 不在作用域内的对象被标记为删除。
- 删除被标记的对象并将内存返回给堆。

GC是个费时的操作，堆上的对象越多，代码中的引用数越多，GC就越费时。

#### 何时会触发GC

三种情况下会触发GC：

- 堆分配时堆上的可用内存不足时触发GC。
- GC会不时的自动运行（频率因平台而异）。
- 手动强制调用GC

#### GC的问题

现在我们了解了GC在Unity内存管理中的作用，我们可以考虑可能发生的问题类型。

最明显的问题是GC可能花费相当长的时间来运行。 如果堆上有很多对象和大量的对象引用要检查，则检查所有这些对象的过程可能很慢。 这可能会导致我们的游戏卡顿或运行缓慢。

另一个问题是GC可能在不合时宜的时刻被触发。 如果CPU在我们游戏的性能关键部分已经满负荷了，那此时即使是少量的GC额外开销也可能导致我们的帧速率下降和性能问题。

另一个不太明显的问题是**堆碎片**。当从堆中分配内存时，会根据必须存储的数据大小从不同大小的块中的可用空间中获取内存。当这些内存块返回到堆时，堆可能分成很多由分配块分隔的小空闲块。这意味着虽然可用内存总量可能很高，但由于碎片化太过严重而无法分配一块连续的大内存块。导致GC被触发或不得不扩大堆大小。

堆内存碎片化有两个后果，一是游戏内存大小会远高于实际所需要的大小，二是GC会被更频繁的触发。 有关堆碎片的更详细讨论，请参阅[这个Unity性能最佳实践指南](https://docs.unity3d.com/Manual/BestPracticeUnderstandingPerformanceInUnity4-1.html)。

**减少GC的影响**

概括的说，可以通过以下三中方式来减少GC对我们游戏的影响：

- 减少GC的时间
- 减少GC的频率
- 故意触发GC，以避开游戏运行的性能关键点，比如加载场景时

基于这些考虑，我们可以使用三种策略：

- 我们可以组织我们的游戏使其更少的堆分配和更少的对象引用。 堆上更少的对象和更少的引用 检查意味着当GC触发时，运行时间更少。
- 我们可以减少堆分配和释放的频率，特别是在性能点。 更少的分配和释放意味着更少的触发GC。 这也降低了堆碎片的问题。
- 我们可以尝试手动触发GC和扩展堆大小以便GC可控并在合适的时候触发。这个方法更难且不可靠，但作为整体内存管理策略的一部分，可以减少GC的影响。

#### 减少垃圾的产生量

可以使用一些技术来帮助我们减少代码中生成的垃圾量

**缓存**

如果我们的代码重复调用产生堆分配的函数，然后丢弃结果，这将产生不必要的垃圾。 对此，我们应该存储对这些对象的引用并复用它们。 这种技术被称为**缓存**。

**不要在频繁调用的函数中分配**

如果我们需要在MonoBehaviour中分配堆内存，在频繁调用的函数里分配是最糟糕的。比如 每帧调用的函数*Update()*和*LateUpdate()*，在这些地方分配，垃圾将非常快的累积。我们应该尽可能在Start() 或 Awake() 里缓存这些对象的引用，或者确保分配内存的代码只在需要的时候被运行。

**清空容器**

创建容器类会引起堆分配,如果在代码中发现多次创建同一个容器变量,则应该缓存该容器引用并在重复创建的地方使用 Clear()操作来替代

**对象池**

即使减少了脚本中的堆分配,在运行时大量对象的创建和销毁依然会引起GC问题. [对象池是一种通过重用对象而不是重复创建和销毁对象来减少分配和释放的技术](https://en.wikipedia.org/wiki/Object_pool_pattern).对象池在游戏中广泛使用,最适合于频繁产生和销毁类似对象的情况;,例如,当枪射击子弹时.

#### 引起不必要堆分配的常见原因

我们知道局部的,值类型的变量被分配在栈上,其他的都在堆上分配.但是很多情况下的堆分配可能让人惊讶.我们来看看一些不必要的堆分配的常见原因,并考虑如何最好地减少这些。

**字符串**

- 减少不必要的字符串创建。 如果多次使用相同的字符串值，应该创建一次该字符串并缓存该值。
- 减少不必要的字符串操作。 例如，如果有一个经常更新的Text组件，并且包含一个连接的字符串，可以考虑将它分成两个Text组件。
- 如果必须在运行时构建字符串，应该使用StringBuilder类。
- 当不在需要调试时,立即删除对Debug.Log()的调用。即使没有输出任何内容，对Debug.Log()的调用依然会被执行。调用Debug.Log() 创建和处理至少一个字符串。

**Unity函数调用**

调用一些Unity函数会产生堆分配，因此应谨慎使用以避免产生不必要的垃圾。

每次访问返回值为数组的Unity函数时，都会创建一个新的数组，并将其作为返回值传递给我们。 这种行为并不总是显而易见的或可预期的，特别是当函数是[访问器](https://msdn.microsoft.com/en-us/library/ms228503.aspx)的时候（例如  [Mesh.normals](https://docs.unity3d.com/ScriptReference/Mesh-normals.html?_ga=2.28863228.1705087000.1501465120-913292091.1467768613)）。解决这种问题就是在迭代前先把系统函数返回值缓存起来。 要检查一个GameObject的标签的值而不产生垃圾，我们可以使用 **GameObject.CompareTag()**。

*GameObject.CompareTag*并不是唯一的，很多Unity的函数都有无堆分配的替代版本。比如可以使用**Input.GetTouch()** 和**Input.touchCount** 替换 **Input.touches，** 或者使用**Physics.SphereCastNonAlloc()** 替换 **Physics.SphereCastAll()。**

**装箱**

装箱会产生垃圾源于其后台操作。当一个值类型变量被装箱时，Unity在堆上创建一个临时的System.Object来包装值类型变量。 一个System.Object是一个引用类型的变量，所以当这个临时对象被处理掉时会产生垃圾。 最好的做法是尽可能避免装箱，并删除导致装箱的任何函数调用。

**协程**

调用StartCoroutine()会产生少量的垃圾，因为Unity必须创建一些管理协程的实例的类。为了减少这种方式产生的垃圾，必须在性能热点运行的协程应该提前启动，当使用可能包含对StartCoroutine()的延迟调用的嵌套协程时，我们应特别小心。

协程中的yield语句不会自己产生堆分配; 然而，我们传递给yield语句的值可能会产生不必要的堆分配。 例如，以下代码会产生垃圾：

```
yield return 0;
```

该代码产生垃圾，因为int变量0被装箱。 在这种情况下，如果我们希望只是等待一个帧而不会导致任何堆分配，那么最好的方法是使用以下代码：

```
yield return null;
```

协程的另一个常见错误是在多次使用相同的值时使用了new操作， 例如，以下代码将在循环迭代时每次都重复创建和销毁一个WaitForSeconds对象：

```
while (!isComplete)  {   yield return new WaitForSeconds(1f);  }
```

如果缓存和复用WaitForSeconds对象，就能减少垃圾的产生量，请看以下示例代码：

```
WaitForSeconds delay = new WaitForSeconds(1f);
while (!isComplete) {	yield return delay;	}
```

如果我们的代码由于协程而产生大量垃圾，我们可能考虑使用除协程之外的其他东西来重构我们的代码。 重构代码是一个复杂的问题，每个项目都是独一无二的，但是有一些常用的手段或许对协程问题有帮助。 例如，如果我们主要使用协同程序来管理时间，我们可以简单地在一个Update()函数中记录时间。 如果我们主要使用协同程序来控制游戏中发生的事情的顺序，我们可以创建某种消息系统来允许对象进行通信。 一个方法不能解决所有问题，但是有必要记住，在代码中可以有多种方法来实现相同的事情。

**foreach循环**

在Unity5.5之前的版本中，使用foreach遍历数组之外的所有集合，在循环终止时都会产生垃圾，这是因为其后台的装箱操作。当循环开始并且循环终止时，一个System.Object对象被分配在堆上。 **Unity 5.5中已修复此问题。**

**函数引用**

函数引用，无论是引用[匿名函数](https://msdn.microsoft.com/en-us/library/0yw3tz5k.aspx)还是命名函数，都是Unity中的引用类型变量。 它们将导致堆分配。 将匿名函数转换为 [闭包](https://en.wikipedia.org/wiki/Closure_(computer_programming))（匿名函数可在其创建时访问范围中的变量）显著增加了内存使用量和堆分配数量。

函数引用和闭包如何分配内存的精确细节因平台和编译器设置而异，但是如果GC是一个问题，那么最好在游戏过程中尽量减少使用函数引用和闭包。 [这个Unity性能最佳实践指南](https://docs.unity3d.com/Manual/BestPracticeUnderstandingPerformanceInUnity4-1.html) 在这个主题上有更多的技术细节。

**LINQ和正则表达式**

LINQ和正则表达式由于在后台会有装箱操作而产生垃圾。在有性能要求的时候最好不使用。 同样，[这个Unity性能最佳实践指南](https://docs.unity3d.com/Manual/BestPracticeUnderstandingPerformanceInUnity4-1.html) 提供了有关此主题的更多技术细节。

**构建代码以最小化GC的影响**

代码的构建方式可能会影响GC。即使代码中没有堆分配，也有可能增加GC的负担。

可能增加GC的负担之一是要求它检查它不应该检查的东西。**Structs是值类型变量，但是如果有一个包含引用类型变量的struct，那么垃圾收集器必须检查整个结构体**。 如果有大量这样的结构体，那么垃圾回收器将增加大量额外的工作。

**定时GC**

**手动强制GC**

最后，我们可能希望自己触发GC。 如果我们知道堆内存已被分配但不再使用（例如，如果我们的代码在加载资源时生成垃圾），并且我们知道垃圾收集冻结不会影响播放器（例如，当加载界面还显示时），我们可以使用以下代码请求GC：

System.GC.Collect();