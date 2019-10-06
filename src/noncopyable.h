/*
 * noncopyable.h
 * Copyright (C) 2019 qingyun <qingyun.oracle@gmail.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

namespace JerryFish {

/**
 * @brief 对象无法拷贝,赋值
 */
class Noncopyable {
public:
    /**
     * @brief 默认构造函数
     */
    Noncopyable() = default;

    /**
     * @brief 默认析构函数
     */
    virtual ~Noncopyable() = default;

    /**
     * @brief 拷贝构造函数(禁用)
     */
    Noncopyable(const Noncopyable&) = delete;

    /**
     * @brief 赋值函数(禁用)
     */
    Noncopyable& operator=(const Noncopyable&) = delete;
};

}

#endif /* !NONCOPYABLE_H */
