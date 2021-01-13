# `malloc`&`free`开发



**注意：由于是实现`malloc`函数，头文件就不能包括`stdlib.h`，因为这个头文件中包含了`malloc`函数！**



## 实现原理

>  使用了一个结构体用来表示分配的内存块：
>
> 1. 链接下一个内存块的指针
> 2. 标识是否存储

**`malloc`核心思想**：

1. 初始时系统未分配，参数也处于未设置的状态，即全局参数在第一次分配的时候进行初始化
2. 初始时，请求一个内存块
3. 之后请求分配内存块时，先在内存块中寻找是否有能够装下的内存，有则分配，没有则请求一个新的内存块，成功分配，没有失败

**`free`核心思想**：将分配过的内存块的状态改变为未分配的状态，之后可以继续使用



## 代码解读



### 1. 核心实现`malloc.c`



#### 头文件

`assert.h`：程序使用了`assert`来中断程序

`string.h`：程序使用到了字符相关的操作

`sys/types.h`：程序使用到了如`size_t`的定义

`unistd.h`：程序使用到了类UNIX系统操作原语如`sbrk`



#### 结构体&定义

```cpp
struct block_meta {
    size_t size;
    struck block_meta *next; //链表形式存储
    int free; //0，标识不可用；1，标识可用
    int magic; //只是用来debug
}
#define META_SIZE sizeof(struck block_meta) //这个宏定义一定要注意
void *global_base = NULL;
```



#### 函数

1. `nofree_malloc` 判断是否释放空间

```cpp
void *nofree_malloc(size_t size)
```

> 声明：
>
> `sbrk`在每次我们调用的时候，都调用了一些额外的空间
>
> **而现在还没有资料说明如何释放这些多出来的空间**

> `size_t` 一种用来记录大小的数据类型
>
> 保存的是一个整数，通常我们使用`sizeof()`的返回结果就是`size_t`
>
> **注意：保存在`sys/types.h`**头文件

```cpp
void *p = sbrk(0); //在调用程序时，动态分配空间，使用sbrk函数分配更多的空间，sbrk函数在内核的管理下将虚拟地址空间映射到内存
void *request = sbrk(size); //这里是需求的大小，后面的size是传入的需要的分配空间大小
if(request==(void*) -1) {
    return NULL; //分配失败
} else {
    assert(p == request); //线程不安全，如果内存分配头指针==分配指针，表示内存已经释放；如果未释放，表示nofree，并报错
    return p;//内存释放了就返回此时的p指针
}
```

> `int brk(void *end_data_segment)` //移动指针分配空间，释放空间
>
> `void* sbrk(ptrdiff_t increment)` // 分配内存空间，返回指定大小的空间地址

> `assert(int expression)` //先计算表达式，**如果值为false**，先stderr打印一条出错信息，然后通过调用abort来种植程序运行
>
> **注意：保存在`assert.h`头文件**

2. `find_free_block` 寻找空闲内存

```cpp
struct block_meta *find_free_block(struck block_meta **last, size_t size) 
```

> 传入的是内存块的地址指针指针

```cpp
struct block_meta *current = global_base; //当前的内存块
while(current && !(current->free && current-size >= size)) { //当前current存在，不满足(当前找到的内存块空闲且大小大于需要分配的大小)条件继续寻找
    *last = current; // 当前
    current = current -> next; //下一个内存块
}
return current; //找到的结果（需要判断具体的内容是否可用）

```

3. `request_space` 请求空间

```cpp
struct block_meta *request_space(struct block_meta* last, size_t size)
```

> 传入地址

```cpp
struct block_meta *block;
block = sbrk(0);
/******************只是证明可以分配*******************/
void *request = sbrk(size + META_SIZE); //size是当前想要分配的内存，需要加上META_SIZE是已经存过的内存
assert((void*)block == request); //如果线程不安全
if(request == (void*) -1) return NULL; //分配失败
/**************************************************/

/******************开始分配的语句*********************/
if(last) last->next = block; //第一次请求时last==NULL跳过此步，之后每一次使用指针链接当前到指定block，也就是内存的开头
block->size = size;
block->next = NULL;
block->free = 0; //标识占用
block->magic = 0x12345678; //测试的地址
return block;
```

4. **`malloc`** 分配空间

```cpp
void *malloc(size_t size)
```

> 核心思想：
>
> 注意：第一次分配时`global_base`为`NULL`，此时需要调用`request_space `请求空间和 设置`global_base`
>
> 1. 如果我们能够找到一个空闲块，就使用它
> 2. 如果不能，继续调用`request_space`请求空间

```cpp
struct block_meta *block;

if(size<=0) return NULL; //1. 非法输入
if(!global_base) {
    block = request_space(NULL,size);
    if(!block) return NULL; //2. 无法分配
    global_base = block;
} else {
    struck block_meta *last = global_base;
    block = find_free_block(&last, size);//在内存空间中找
    if(!block) {
        block = request_space(last,size);//请求新的内存空间
        if(!block) return NULL; //3. 请求无效
    } else {
        //值得注意的：考虑冲突的快
        block->free = 0; //标识block已经被占用
        block->magic = 0x777777777; 
    }
}

return (block+1); //4. 请求成功？返回分配成功的内存块的下一个指针
```

5. `calloc`连续分配

```cpp
void *calloc(size_t num, size_t size)
```

> 在内存的区中分配`num`个长度为`size`的连续空间，函数返回一个指向分配起始地址的；如果分配不成功，返回`NULL`。

```cpp
size_t size = num * size;
void *ptr = malloc(size); //分配
memset(ptr, 0, size); //初始化调用malloc分配的空间（设置为0)
return ptr;
```

6. `get_block_ptr`将分配指针作为内存块指针返回

```cpp
struct block_meta *get_block_ptr(void *ptr)
```

```cpp
return (struct block_meta*)ptr - 1;//因为malloc中返回的是分配成功的内存块的下一个指针，这里需要-1才能指向真正分配的内存块的指针
```

7. **`free`**释放内存

```cpp
void free(void *ptr)
```

```cpp
if(!ptr) return NULL; //释放不存在的指针？们都没有

struct block_meta* block_ptr = get_block_ptr(ptr);//分配的内存块指针
assert(block_ptr->free == 0);//如果这个内存块并没有被占用，实际上是没有成功的
assert(block_ptr->magic == 0x7777777 || block_ptr->magic == 0x12345678);//如果不是这两个值，就代表根本block没有进行任何的内存分配的操作！
block_ptr->free = 1; //内存标识为1，空闲啦
block_ptr->magic = 0x555555555; 
```

8. `realloc` 动态调整已经分配的内存块的大小

```cpp
void *realloc(void *ptr, size_t size)
```

```cpp
if(!ptr) return malloc(size); //如果未分配，直接分配一个malloc

//需要malloc一个新地址，然后是否原来的旧空间并且干掉野指针，记得偷偷复制原来的数据，对用户透明
void *new_ptr;
new_ptr = malloc(size);
if(!new_ptr) return NULL;
memcpy(new_ptr, ptr, block_ptr->size); //从原块到目的块
free(ptr);
return new_ptr;
```

### 2. 调用实例

#### `shell命令行模式`

```cpp
char **env = malloc(2 * size(char *));
env[0] = malloc(100 * size(char));//这个是系统的
sprintf(env[0], "LD_PRELOAD=./malloc.o");//后面的malloc.o对应编译malloc后的文件，你可以改名为对应的lib文件；替换环境中定义的malloc链接的库
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

将`malloc.c`编译后的文件打包直接使用



## 面向对象设计

需要吗？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？不需要吧，打扰了

