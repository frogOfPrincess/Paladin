//
//  sceneparser.hpp
//  Paladin
//
//  Created by SATAN_Z on 2019/11/11.
//

#ifndef sceneparser_hpp
#define sceneparser_hpp

#include "transformcache.h"
#include <fstream>
#include "core/integrator.hpp"


PALADIN_BEGIN

USING_STD

class SceneParser {
    
public:
    void loadFromJson(const std::string &fn) {
        try {
            std::ifstream fst;
            fst.open(fn.c_str());
            stringstream buffer;
            buffer << fst.rdbuf();
            string str = buffer.str();
            nloJson json = nloJson::parse(str);
            parse(json);
        } catch (const std::exception &exc) {
            cout << exc.what();
            return;
        }
    }
    
    const TransformCache &getTransformCache() const {
        return _transformCache;
    }
    
    void parse(const nloJson &);
    
    void parseShapes(const nloJson &);
    
    Sampler * parseSampler(const nloJson &param, Film *);
    
    Camera * parseCamera(const nloJson &, Film *);
    
    Integrator * parseIntegrator(const nloJson &,Sampler * sampler, Camera * camera);
    
    void parseMaterials(const nloJson &);
    
    void parseMediums(const nloJson &);
    
    Filter * parseFilter(const nloJson &);
    
    void parseLights(const nloJson &);
    
    // 解析简单物体，球体，圆柱，圆锥等
    void parseSimpleShape(const nloJson &data, const string &type);
    
    // 解析模型
    void parseTriMesh(const nloJson &data);
    
    // 解析clone物体
    void parseClonal(const nloJson &data);
    
    shared_ptr<Aggregate> parseAccelerator(const nloJson &);
    
    Film * parseFilm(const nloJson &param, Filter *);
    
private:
    
    void addMaterialToCache(const string &name, const shared_ptr<const Material> &material) {
        // todo
        _materialCache[name] = material;
    }
    
    shared_ptr<const Material> getMaterial(const nloJson &name) {
        if (name.is_null()) {
            return nullptr;
        }
        auto iter = _materialCache.find(name);
        if (iter == _materialCache.end()) {
            return nullptr;
        }
        return _materialCache[name];
    }
    
    void addMediumToCache(const string &name, const shared_ptr<const Medium> &medium) {
        _mediumCache[name] = medium;
    }
    
    MediumInterface getMediumIntetface(const nloJson& data) {
        if (data.is_null() || data.size() == 0) {
            return nullptr;
        }
        if (data.size() == 1) {
            string mediumName = data[0];
            const Medium * inside = getMedium(mediumName).get();
            return MediumInterface(inside);
        } else if (data.size() == 2) {
            string insideName = data[0];
            string outsideName = data[1];
            const Medium * inside = getMedium(insideName).get();
            const Medium * outside = getMedium(outsideName).get();
            return MediumInterface(inside, outside);
        }
        return nullptr;
    }
    
    shared_ptr<const Medium> getMedium(const nloJson &name) {
        if (name.is_null()) {
            return nullptr;
        }
        auto iter = _mediumCache.find(name);
        if (iter == _mediumCache.end()) {
            return nullptr;
        }
        return _mediumCache[name];
    }
    
    void addPrimitivesToCloneMap(const std::string &key, const vector<shared_ptr<Primitive>> &value) {
        _cloneMap[key] = value;
    }
    
    const vector<shared_ptr<Primitive>> & getPrimitives(const string &key) {
        return _cloneMap[key];
    }
    
private:
    
    TransformCache _transformCache;
    
    shared_ptr<Aggregate> _aggregate;
    
    unique_ptr<Integrator> _integrator;
    
    unique_ptr<Scene> _scene;
    
    vector<shared_ptr<Light>> _lights;
    
    vector<shared_ptr<Primitive>> _primitives;
    
    // 克隆map，key为对象名，value为片元列表
    map<string, vector<shared_ptr<Primitive>>> _cloneMap;
    
    // 先用map储存着，待后续优化todo
    map<string, shared_ptr<const Material>> _materialCache;
    
    // 先用map储存着，待后续优化
    map<string, shared_ptr<const Medium>> _mediumCache;
};

PALADIN_END

#endif /* sceneparser_hpp */
