//
//  medium.hpp
//  Paladin
//
//  Created by SATAN_Z on 2019/7/16.
//  Copyright © 2019 Zero. All rights reserved.
//


/**
 *
 * 就像BSDF描述了场景中表面的反射一样，介质类的实现代表了表面之间发生的散射;
 * 例子包括大气散射效应，如雾霾，吸收在彩色玻璃窗，或散射的脂肪球在一瓶牛奶。
 * 从技术上讲，所有这些现象都是由于大量微观粒子的表面相互作用造成的，
 * 尽管找到一种比单独考虑它们更简便的建模方法更为可取。
 * 在本章描述的模型中，粒子被认为是如此之多，以至于它们可以用统计分布而不是显式计数来表示。
 *
 *
 * 
 */

#ifndef medium_hpp
#define medium_hpp

#include "core/header.h"
#include "core/cobject.h"

PALADIN_BEGIN
/**
 *
 * 正如描述表面散射的BSDF模型种类繁多，许多相函数也得到了发展。
 * 这些模型从参数化模型(可用于将具有少量参数的函数拟合到测量数据)
 * 到基于从具有已知形状和材料的粒子(如球形水滴)推导出散射辐射分布的分析模型。
 * 相函数是一个一维函数，参数为cosθ，其中θ为入射方向与出射方向的夹角
 * 相函数有互换性，任意交换两个向量，函数值保持不变
 * 相函数满足以下表达式
 *       ∫[sphere]p(ωo,ωi) = 1
 * 所以各向同性的相函数满足
 * p(ωo,ωi) = 1/4π
 *
 * Henyey和Greenstein(1941)开发了一种广泛使用的相位函数。
 * 这种相位函数是专门设计的，以方便适合测量散射数据。
 * 一个单独的参数g(称为不对称参数)控制着散射光的分布
 *
 * 表达式如下
 *
 *                1              1 - g^2
 * P_hg(cosθ) = ---- * ----------------------------
 *               4π     (1 + g^2 + 2g cosθ)^(3/2)
 * 
 */
class PhaseFunction {
public:
   
    virtual ~PhaseFunction() {

    }

    virtual Float p(const Vector3f &wo, const Vector3f &wi) const = 0;

    virtual Float sample_p(const Vector3f &wo, Vector3f *wi,
                           const Point2f &u) const = 0;

    virtual std::string toString() const = 0;
};

// 介质
class Medium {
public:
    // Medium Interface
    virtual ~Medium() {}
    
    virtual Spectrum Tr(const Ray &ray, Sampler &sampler) const = 0;
    
    virtual Spectrum sample(const Ray &ray, Sampler &sampler,
                            MemoryArena &arena,
                            MediumInteraction *mi) const = 0;
};

class HenyeyGreenstein : public PhaseFunction {
public:
    HenyeyGreenstein(Float g) 
    : _g(g) {

    }

    virtual Float p(const Vector3f &wo, const Vector3f &wi) const override;

    virtual Float sample_p(const Vector3f &wo, Vector3f *wi,
                   const Point2f &sample) const override;

    virtual std::string toString() const override {
        return StringPrintf("[ HenyeyGreenstein g: %f ]", _g);
    }

private:
    const Float _g;
};

/**
 * Henyey和Greenstein开发的phase函数，表达式如下
 * 
 *                1              1 - g^2
 * P_hg(cosθ) = ---- * ----------------------------
 *               4π     (1 + g^2 + 2g cosθ)^(3/2)
 *  
 */
inline Float phaseHG(Float cosTheta, Float g) {
    Float denom = 1 + g * g + 2 * g * cosTheta;
    return Inv4Pi * (1 - g * g) / (denom * std::sqrt(denom));
}

// 两个介质的相交处，nullptr表示真空
struct MediumInterface {
    MediumInterface() : inside(nullptr), outside(nullptr) {}

    MediumInterface(const Medium *medium) : inside(medium), outside(medium) {

    }

    MediumInterface(const Medium *inside, const Medium *outside)
    :inside(inside),
    outside(outside) {

    }
    
    bool isMediumTransition() const {
        return inside != outside;
    }
    // 内部的介质
    const Medium *inside;
    
    // 外部的介质
    const Medium *outside;
};

PALADIN_END

#endif /* medium_hpp */
