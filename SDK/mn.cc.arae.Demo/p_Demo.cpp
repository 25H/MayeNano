#include "SDK_MN.hpp"

#include <deque>
#include <vector>
#include <string>
#include <iostream>




std::vector<mn::SEARCH_RESULT_ITEM> search_results;

std::deque<std::string> search_string_pool;// 字符串池
const char* ssp(const std::string& s) {
    return search_string_pool.emplace_back(s).c_str();
}
void search_clean() {
    std::vector<mn::SEARCH_RESULT_ITEM>().swap(search_results);
    std::deque<std::string>().swap(search_string_pool);
}

unsigned char _testImgBlack[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44,
0x52, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x20, 0x01, 0x03, 0x00, 0x00, 0x00, 0x49,
0xB4, 0xE8, 0xB7, 0x00, 0x00, 0x00, 0x06, 0x50, 0x4C, 0x54, 0x45, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0xA5, 0x67, 0xB9, 0xCF, 0x00, 0x00, 0x00, 0x01, 0x74, 0x52, 0x4E, 0x53, 0x00,
0x40, 0xE6, 0xD8, 0x66, 0x00, 0x00, 0x00, 0x19, 0x49, 0x44, 0x41, 0x54, 0x08, 0xD7, 0x63,
0xF8, 0xC3, 0xC0, 0x60, 0x8F, 0x9B, 0x20, 0x06, 0xB0, 0x3F, 0xC0, 0x43, 0x10, 0x06, 0xF8,
0x5D, 0x00, 0x00, 0x5A, 0x19, 0x14, 0x2F, 0xD4, 0x17, 0x7F, 0x9C, 0x00, 0x00, 0x00, 0x00,
0x49, 0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82 };


// 释放上次搜索的缓存
void _MN_Search_Free_Callback() {
    search_clean();
}

// 关键词变更
mn::SEARCH_RESULT_LIST _MN_Search_Callback(const char* prefix, const char* key, const char* key_low) {

    mn::SEARCH_RESULT_ITEM a;
    a.boost = 300;
    a.iconBuf = (const unsigned char*)&_testImgBlack;
    a.iconBufLen = sizeof(_testImgBlack);

    // 设置新key
    a.name = "new key";
    a.nameSub = "set key: demo new-key";
    a.action = mn::code::searchItemAction::NewKey;
    a.param_0 = "demo new-key";
    --a.boost;
    search_results.emplace_back(a);

    a.action = mn::code::searchItemAction::Nil;

    // MayeNano版本号
    a.name = "GetVersion";
    a.nameSub = ssp(mn::Api::GetVersion());
    --a.boost;
    search_results.emplace_back(a);

    // 自身Id
    a.name = "GetSelfId";
    a.nameSub = ssp(mn::Api::GetSelfId());
    --a.boost;
    search_results.emplace_back(a);

    // 自身目录
    a.name = "GetSelfDir";
    a.nameSub = ssp(mn::Api::GetSelfDir());
    --a.boost;
    search_results.emplace_back(a);


    // 填充一点测试数据
    for (size_t i = 0; i < 100; i++) {
        mn::SEARCH_RESULT_ITEM a;
        a.name = ssp(std::string("aaa-") + std::to_string(i));
        a.boost = i;
        a.icon = "pkg:/img/plugin/_testImgRed.png";
        if (i == 98) {
            a.icon = "_testImgBlue.png";
        }
        if (i == 99) {
            a.iconBuf = (const unsigned char*)&_testImgBlack;
            a.iconBufLen = sizeof(_testImgBlack);
        }
        search_results.emplace_back(a);
    }

    // 返回
    return {
        .items = search_results.data(),
        .count = (unsigned int)search_results.size(),
    };
}



// 此函数用作事件通知
mn::_MN_RESULT _MN_Notify(unsigned int msg, void* param1, void* param2) {
    
    switch (msg) {
        
        // 插件事件-被载入
        case mn::code::notify::Load:
        {
            return Null;
        }
        
        // 插件事件-销毁(被禁用不会调用此事件)
        case mn::code::notify::Destroy:
        {
            return Null;
        }
        
        // 插件事件-被启用，当用户在 设置页 启用插件时，触发此事件
        case mn::code::notify::Enable:
        {
            return Null;
        }
        
        // 插件事件-被禁用，当用户在 设置页 禁用插件时，触发此事件
        case mn::code::notify::Disable:
        {
            return Null;
        }
        
        // 插件事件-打开选项，当用户点击选项，触发此事件
        case mn::code::notify::Open_Option:
        {
            return Null;
        }
        
        // 插件事件-搜索-初始化，插件类型为 mn::code::type::Search 时，触发此事件，并要求设置callback触发条件信息
        case mn::code::notify::Search_Init:
        {
            auto info = (mn::_SEARCH_INFO*)param1;

            info->name = "Demo";
            info->desc = "Demo-Demo-搜索";

            info->icon = "demo.png";
            info->boost = 100;

            info->keys = "demo\0d \0d\0\0";
            info->regex = Null;
            info->fn = _MN_Search_Callback;
            info->fn_free = _MN_Search_Free_Callback;

            return True;
        }
        

    }


    return Null;
}




// 插件基本信息
__MN_PLUGIN_IN__ mn::_PLUGIN_INFO _MN_Info(void* fn, void* ctx) {

    mn::_MN_API_FN = (mn::PFN_API_FN)fn;
    mn::_MN_CTX = ctx;

    return {
        .sdk_version    = MN_SDK_VERSION,

        .name           = "Demo",
        .description    = "description",

        .version        = "1.0.0.1",
        .author         = "author",
        .email          = "email",
        .homepage       = "homepage",

        .type           = mn::code::type::Search,
        .fnNotify       = _MN_Notify,

    };

}



