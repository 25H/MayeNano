
#include <windows.h>
#include "SDK_MN.hpp"

#include <deque>
#include <vector>
#include <string>
#include <iostream>




std::vector<mn::_MN_PLUGIN_SEARCH_RESULT_ITEM> search_results;

std::deque<std::string> search_string_pool;// 字符串池
const char* ssp(const std::string& s) {
    return search_string_pool.emplace_back(s).c_str();
}
void search_clean() {
    std::vector<mn::_MN_PLUGIN_SEARCH_RESULT_ITEM>().swap(search_results);
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



mn::_MN_PLUGIN_SEARCH_RESULT_LIST _MN_Search_Callback(bool isClear, const char* prefix, const char* key, const char* key_low) {

    // 释放搜索缓存数据
    if (isClear) {
        search_clean();
        return {};
    }

    for (size_t i = 0; i < 100; i++) {
        mn::_MN_PLUGIN_SEARCH_RESULT_ITEM a;
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

    return {
        .items = search_results.data(),
        .count = (unsigned int)search_results.size(),
    };
}



// 此函数用作事件通知
mn::_MN_RESULT _MN_Notify(unsigned int msg, void* param1, void* param2) {

    switch (msg) {

        // 插件事件-被载入
        case _MN_PLUGIN_EVENT_LOAD:
        {
            return NULL;
        }

        // 插件事件-销毁(被禁用不会调用此事件)
        case _MN_PLUGIN_EVENT_DESTROY:
        {
            return NULL;
        }

        // 插件事件-被启用，当用户在 设置页 启用插件时，触发此事件
        case _MN_PLUGIN_EVENT_ENABLE:
        {
            return NULL;
        }

        // 插件事件-被禁用，当用户在 设置页 禁用插件时，触发此事件
        case _MN_PLUGIN_EVENT_DISABLE:
        {
            return NULL;
        }

        // 插件事件-配置被更改，当用户更改了设置时，触发此事件
        case _MN_PLUGIN_EVENT_CONFIG_CHANGE:
        {
            return NULL;
        }

        // 插件事件-搜索-初始化，插件类型为 _MN_PLUGIN_TYPE_SEARCH 时，触发此事件，并要求设置callback触发条件信息
        case _MN_PLUGIN_EVENT_SEARCH_INIT:
        {

            auto info = (mn::_MN_PLUGIN_SEARCH_INFO*)param1;

            info->name = "Demo";
            info->desc = (char*)u8"Demo-Demo-搜索";

            info->icon = "demo.png";
            info->boost = 100;

            info->keys = (char*)u8"demo\0d \0d\0\0";
            info->regex = NULL;
            info->fn = &_MN_Search_Callback;

            return TRUE;
        }

    }

    return NULL;
}




// 插件基本信息
__MN_PLUGIN_IN__ mn::_MN_PLUGIN_INFO _MN_Info(void* fn, void* ctx) {

    mn::_MN_API_FN = (mn::_PFN_API_FN)fn;
    mn::_MN_CTX = ctx;

    return {

        .name           = (char*)u8"Demo",
        .description    = (char*)u8"description",
        .version        = (char*)u8"1.0.0.1",
        
        .author         = (char*)u8"author",
        .email          = (char*)u8"email",
        .homepage       = (char*)u8"homepage",
        
        .type           = _MN_PLUGIN_TYPE_SEARCH,
        .fnNotify       = &_MN_Notify,

    };

}



