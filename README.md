# `mymalloc`&`myfree`开发

## 实现原理

>  使用了一个结构体用来表示分配的内存块：
>
> 1. 链接下一个内存块的指针
> 2. 标识是否存储

**`mymalloc`核心思想**：

1. 初始时系统未分配，参数也处于未设置的状态，即全局参数在第一次分配的时候进行初始化
2. 初始时，请求一个内存空间
3. 之后请求分配内存时，先在当前内存空间中的寻找是否有能够装下的内存块，有则分配，没有则请求一个新的内存空间，成功分配，没有失败

**`free`核心思想**：将分配过的内存块的状态改变为未分配的状态，之后可以继续使用



## 代码解读



### 1. 核心实现`mymalloc.c`



#### 头文件

`assert.h`：程序使用了`assert`来中断程序

`string.h`：程序使用到了字符相关的操作

`sys/types.h`：程序使用到了如`size_t`的定义

`unistd.h`：程序使用到了类UNIX系统操作原语如`sbrk`



#### 结构体&定义

```cpp
struct my_block {
    size_t size;
    struck my_block *next; //链表形式存储
    int free; //0，标识不可用；1，标识可用
}
static size_t g_size = sizeof(struck my_block) //表示已经分配的内存空间
static void *g_base = NULL;//表示当前系统的内存块
```



#### 函数

1. `find_free_block` 寻找空闲内存块

```cpp
struct my_block *find_free_block(struck my_block **last_block, size_t size) 
```

> 传入的是内存块的地址指针指针

```cpp
struct my_block *current = g_base; //当前的内存块
//1. 当前内存块为NULL即无法满足条件，无法寻找跳出循环，结果无法找到
//2. 当前内存块存在，同时内存块能够使用，内存块的大小可以满足使用，结束寻找
while(current && !(current->free && current-size >= size)) {
    *last_block = current; // 当前
    current = current -> next; //下一个内存块
}
return current; //找到的结果（需要判断具体的内容是否可用）

```
> `int brk(void *end_data_segment)` //移动指针分配空间，释放空间
> `void* sbrk(ptrdiff_t increment)` // 分配内存空间，返回分配后的指针位置

> `assert(int expression)` //先计算表达式，如果值为`false`，先`stderr`打印一条出错信息，然后通过调用`abort`来终止程序运行
> 注意：保存在`assert.h`头文件

3. `find_new_space` 请求空间

```cpp
struct my_block *find_new_space(struct my_block* last_block, size_t size)
```

> 传入地址

```cpp
struct my_block *block;
block = sbrk(0);
/******************只是证明可以分配*******************/
void *request = sbrk(size + g_size); //size是当前想要分配的内存，需要加上g_size是已经存过的内存
assert((void*)block == request); //如果线程不安全
if(request == (void*) -1) return NULL; //分配失败
/**************************************************/

/******************开始分配的语句*********************/
if(last_block) last_block->next = block; //第一次请求时last_block==NULL跳过此步，之后每一次使用指针链接当前到指定block，也就是内存的开头
block->size = size;
block->next = NULL;
block->free = 0; //标识占用
return block;
```

4. **`mymalloc`** 分配空间

```cpp
void *mymalloc(size_t size)
```

> 核心思想：
>
> 注意：第一次分配时`g_base`为`NULL`，此时需要调用`find_new_space `请求空间和 设置`g_base`
>
> 1. 如果我们能够找到一个空闲块，就使用它
> 2. 如果不能，继续调用`find_new_space`请求空间

```cpp
struct my_block *block;

if(size<=0) return NULL; //1. 非法输入
if(!g_base) {
    block = find_new_space(NULL,size);//请求内存空间
    if(!block) return NULL; //2. 无法分配
    g_base = block;//空间位置
} else {
    struck my_block *last_block = g_base;
    block = find_free_block(&last_block, size);//在内存空间中找内存块
    if(!block) {
        block = find_new_space(last_block,size);//请求新的内存空间
        if(!block) return NULL; //3. 请求无效
    } else {
        block->free = 0; //标识block已经被占用
    }
}

return (block+1); //4. 请求成功？返回分配成功的内存块的下一个指针
```

5. `mycalloc`连续分配

```cpp
void *mycalloc(size_t num, size_t size)
```

> 在内存的区中分配`num`个长度为`size`的连续空间，函数返回一个指向分配起始地址的；如果分配不成功，返回`NULL`。

```cpp
size_t size = num * size;
void *ptr = mymalloc(size); //分配
memset(ptr, 0, size); //初始化调用mymalloc分配的空间（设置为0)
return ptr;
```

6. `get_block_ptr`将分配指针作为内存块指针返回

```cpp
struct my_block *get_block_ptr(void *ptr)
```

```cpp
return (struct my_block*)ptr - 1;//因为mymalloc中返回的是分配成功的内存块的下一个指针，这里需要-1才能指向真正分配的内存块的指针
```

7. **`myfree`**释放内存

```cpp
void myfree(void *ptr)
```

```cpp
if(!ptr) return NULL; //释放不存在的指针

struct my_block* block_ptr = get_block_ptr(ptr);//分配的内存块指针
assert(block_ptr->free == 0);//如果这个内存块并没有被占用，实际上是没有成功的
block_ptr->free = 1; //内存标识为1，空闲啦
```

8. `realloc` 动态调整已经分配的内存块的大小

```cpp
void *realloc(void *ptr, size_t size)
```

```cpp
if(!ptr) return mymalloc(size); //如果未分配，直接分配一个mymalloc

//需要mymalloc一个新地址，然后是否原来的旧空间并且干掉野指针，记得偷偷复制原来的数据，对用户透明
void *new_ptr;
new_ptr = mymalloc(size);
if(!new_ptr) return NULL;
memcpy(new_ptr, ptr, block_ptr->size); //从原块到目的块
free(ptr);
return new_ptr;
```

### 2. 调用实例

#### `shell命令行模式`

```cpp
char **env = mymalloc(2 * size(char *));
env[0] = mymalloc(100 * size(char));//这个是系统的
sprintf(env[0], "LD_PRELOAD=./mymalloc.o");//后面的mymalloc.o对应编译mymalloc后的文件，你可以改名为对应的lib文件；替换环境中定义的mymalloc链接的库
env[1] = NULL;//防止溢出多出来的空间
```

> `int execve(const char * filename,char * const argv[],char * const envp[]);`
>
> 解释：
>
> 1. 用来执行第一个参数`filename`字符串所代表的文件路径
>
> 1. 第二个参数是利用指针数组来传递给执行文件，并且需要以空指针(NULL)结束的输入参数集合
> 2. 最后一个参数则为传递给执行文件的新环境变量数组

#### 直接调用

将`mymalloc.c`编译后的文件打包直接使用