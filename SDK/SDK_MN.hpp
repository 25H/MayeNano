/**
  * [字符编码]
  * 所有可见字符串编码必须为 UTF-8
  * 
  * 
  * [内存与生命周期约定]
  * 所有数据均由插件持有 MayeNano 仅 Copy 不会主动释放该内存。
  * 要求释放前必须保持有效。
  * 
  * 
  * [线程]
  * callback均来自主线程。
  * API除非明确标识非线程安全，均可多线程调用
  * 
  * 
**/
#ifndef __SDK_MN_HPP__
#define __SDK_MN_HPP__


#include <string>

#ifndef MN_SDK_VERSION
#define MN_SDK_VERSION  2    // SDK版本
#endif

#ifndef Null
#define Null    0
#endif

#ifndef False
#define False   0
#endif

#ifndef True
#define True    1
#endif


#define __MN_PLUGIN_IN__ extern "C" __declspec(dllexport)




// Maye Nano - SDK
namespace mn {


    // 事件码
    namespace code {


        // 插件类型
        namespace type {
            enum : unsigned int {
                Search = 100,// 插件类型-搜索
            };
        }


        // 通知事件 _MN_Notify
        namespace notify {
            enum : unsigned int {

                // 插件事件-被载入
                //         param1: Null
                //         param2: Null
                //         return: 应返回 Null
                Load = 1000,

                // 插件事件-销毁(被禁用不会调用此事件)
                //         param1: Null
                //         param2: Null
                //         return: 应返回 Null
                Destroy = 9999,

                // 插件事件-被启用，当用户在 设置页 启用插件时，触发此事件
                //         param1: Null
                //         param2: Null
                //         return: 应返回 Null
                Enable = 2010,

                // 插件事件-被禁用，当用户在 设置页 禁用插件时，触发此事件
                //         param1: Null
                //         param2: Null
                //         return: 应返回 Null
                Disable = 2020,

                // 插件事件-打开选项，当用户点击选项，触发此事件
                //         param1: Null
                //         param2: Null
                //         return: 无选项应返回 Null
                Open_Option = 3000,


                // 插件事件-搜索-初始化，插件类型为 mn::code::type::Search 时，触发此事件，并要求设置callback触发条件信息
                //         param1: 指向 _SEARCH_INFO 的指针
                //         param2: Null
                //         return: 应返回 True
                Search_Init = 5000,


            };
        }


        // 搜索 Item - Action
        namespace searchItemAction {
            enum : unsigned int {
                Nil     = 0,            // 不执行任何操作
                Copy    = 101,          // 复制                  [ val: param_0 (u8) ]
                NewKey  = 102,          // 设置新的搜索关键词    [ val: param_0 (u8) ]
                Cmd     = 103,          // 执行 cmd 命令         [ val: param_0 (u8) ]
                Run     = 104,          // 执行 运行 命令        [ val: param_0 (u8) ]
            };
        }

    }
    



    using _MN_RESULT   = long long;
    using PFN_API_FN  = void* (*)(void* ctx, unsigned int op, void* p1, void* p2, void* p3, void* p4, void* p5);
    



    // 事件通知函数指针 函数
    // _MN_RESULT _MN_Notify(unsigned int msg, void* param1, void* param2);
    // @param msg    消息 mn::code::notify::***
    // @param param1 参数1
    // @param param2 参数2
    using PFN_Notify = _MN_RESULT(*)(unsigned int msg, void* param1, void* param2);


    // 插件基本信息
    struct _PLUGIN_INFO {

        unsigned int    sdk_version     = Null;      // SDK 版本，如与MayeNano不符则丢弃

        const char*     name            = Null;      // u8 名称
        const char*     description     = Null;      // u8 描述

        const char*     version         = Null;      // u8 版本
        const char*     author          = Null;      // u8 作者
        const char*     email           = Null;      // u8 电子信箱
        const char*     homepage        = Null;      // u8 主页地址

        unsigned int    type            = Null;      // 插件类型，mn::code::type::***

        PFN_Notify      fnNotify        = Null;      // 事件通知函数指针

    };




    // 搜索 结果 Item
    struct SEARCH_RESULT_ITEM {
        unsigned int            boost       = 0;                //    搜索结果权重(用于排序，数值越大越靠前)

        bool                    isWordWrap  = false;            //    显示名称自动换行
        const char*             name        = Null;             // u8 名称
        const char*             nameSub     = Null;             // u8 名称-二级

        const char*             icon        = Null;             //     图标(相对于插件目录)
        const unsigned char*    iconBuf     = Null;             //     图标数据指针[标准图片](优先级高于icon)
        int                     iconBufLen  = Null;             //     图标数据指针长度

        unsigned int            action      = Null;             //     执行动作，mn::code::SearchItemAction::***
        const char*             param_0     = Null;             //     参数 0
        void*                   param_1     = Null;             //     参数 1
    };

    // 搜索 结果 List
    struct SEARCH_RESULT_LIST {
        SEARCH_RESULT_ITEM*     items = Null;  // 指向 SEARCH_RESULT_ITEM 的 Array
        unsigned int            count = Null;  // items Array 的数量
    };



    // 关键词变更执行函数
    // SEARCH_RESULT_LIST _MN_Search_Callback(const char* prefix, const char* key, const char* key_low);
    // @param prefix   匹配成功的前缀，如果是正则匹配成功则参数值为空字符串
    // @param key      关键词[不包含前缀]
    // @param key_low  关键词[不包含前缀](小写)
    using PFN_SEARCH_CALLBACK = SEARCH_RESULT_LIST(*)(const char* prefix, const char* key, const char* key_low);

    // 搜索缓存释放(释放上次搜索的缓存)
    // void _MN_Search_Free_Callback();
    using PFN_SEARCH_FREE_CALLBACK = void (*)();


    // 搜索 信息
    struct _SEARCH_INFO {
        const char*     name            = Null;         // u8 名称
        const char*     desc            = Null;         // u8 描述
        const char*     icon            = Null;         //    图标(相对于插件目录)
        unsigned int    boost           = Null;         //    搜索结果权重(用于排序，数值越大越靠前)

        const char*     keys            = Null;         // u8 关键字[以 \0 作为分隔 \0\0 作为结束符](可选，keys与regex必须保证设置其中一个)

        const char*     regex           = Null;         //    正则匹配(可选，keys与regex必须保证设置其中一个)
        bool            isRegexMatch    = false;        //    正则匹配匹配模式，false=搜索子串匹配，true=全文精准匹配，

        bool            isGlobalResults = false;        //    是否允许出现在全局结果内(性能影响)

        PFN_SEARCH_CALLBACK      fn      = Null;        //    关键词变更执行函数
        PFN_SEARCH_FREE_CALLBACK fn_free = Null;        //    搜索缓存释放(释放上次搜索的缓存)

    };





    inline PFN_API_FN   _MN_API_FN  = Null;        // API Ptr
    inline void*        _MN_CTX     = Null;        // Plugin Context


    namespace Api {

        enum class APIOPCODE : unsigned int {
            GetVersion = 50,        // 获取 Maye Nano 版本号
            GetSelfId = 100,       // 获取自身ID
            GetSelfDir = 110,       // 获取自身目录

            SetCfgItem = 210,       // 写配置项
            GetCfgItem = 211,       // 读配置项
        };

        void* Call(unsigned int op, void* param_1 = Null, void* param_2 = Null, void* param_3 = Null, void* param_4 = Null, void* param_5 = Null) {
            if (!_MN_API_FN || !_MN_CTX) return Null;
            return _MN_API_FN(_MN_CTX, op, param_1, param_2, param_3, param_4, param_5);
        }
        void* Call(APIOPCODE op, void* param_1 = Null, void* param_2 = Null, void* param_3 = Null, void* param_4 = Null, void* param_5 = Null) {
            return Call(static_cast<unsigned int>(op), param_1, param_2, param_3, param_4, param_5);
        }


        std::string GetApiString(APIOPCODE op, size_t bufSize = 256) {
            std::string buf(bufSize, '\0');
            size_t ws = (size_t)Call(op, buf.data(), (void*)buf.size());
            buf.resize((std::min)(ws, buf.size()));
            return buf;
        }


        // 获取 Maye Nano 版本号
        // @return std::string 如:"1.0.0.1"
        std::string GetVersion() {
            return GetApiString(APIOPCODE::GetVersion, 50);
        }

        // 获取自身ID
        // @return std::string 如:"mn.cc.arae.hash"
        std::string GetSelfId() {
            return GetApiString(APIOPCODE::GetSelfId, 512);
        }

        // 获取自身目录
        // @return std::string 如:".\plugin\mn.cc.arae.hash\"
        std::string GetSelfDir() {
            return GetApiString(APIOPCODE::GetSelfDir, 2048);
        }



        // 写配置项
        // @return bool
        bool SetCfgItem(const char* key, const char* val) {
            return (bool)Call(APIOPCODE::SetCfgItem, (void*)key, (void*)val);
        }

        // 读配置项
        // @param key
        // @param delVal 默认值
        // @param bufSize 读取缓冲区尺寸
        // @return std::string
        std::string GetCfgItem(const char* key, const char* delVal = Null, size_t bufSize = 200) {
            std::string buf(bufSize, '\0');
            size_t ws = (size_t)Call(APIOPCODE::GetCfgItem, (void*)key, (void*)delVal, buf.data(), (void*)buf.size());
            buf.resize((std::min)((size_t)ws, buf.size()));
            return buf;
        }

    }


    
}






#endif //__SDK_MN_HPP__
