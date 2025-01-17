#pragma once

#include "BasicTypes.h"
#include "../fastl/vector.h"
#include "../fastl/string.h"
#include "../fastl/unordered_map.h"
#include "../fastl/unordered_set.h"

constexpr U32 InvalidCompileId = 0xffffffff;

using CompileCategoryType = U8; 

enum class CompileCategory : CompileCategoryType
{ 
    Include = 0, 
    ParseClass,
    ParseTemplate,
    InstantiateClass, 
    InstantiateFunction, 
    InstantiateVariable,
    InstantiateConcept,
    CodeGenFunction, 
    OptimizeFunction, 
    //Gather End

    PendingInstantiations,
    OptimizeModule, 
    FrontEnd,
    BackEnd,
    ExecuteCompiler,
    Other,
    //Display End

    RunPass,
    CodeGenPasses,
    PerFunctionPasses,
    PerModulePasses,
    DebugType,
    DebugGlobalVariable,
    Invalid,

    FullCount,
    GatherNone     = Include, 
    GatherBasic    = ParseClass, 
    GatherFrontEnd = CodeGenFunction,
    GatherFull     = PendingInstantiations,
    DisplayCount   = RunPass,
};

constexpr CompileCategoryType ToUnderlying(CompileCategory input){ return static_cast<CompileCategoryType>(input);}

struct CompileUnit
{ 
    CompileUnit(const U32 _unitId = 0u)
        : unitId(_unitId)
        , values()
    {}

    fastl::string name;
    U32 unitId; //filled by the ScoreProcessor
    U32 values[ToUnderlying(CompileCategory::DisplayCount)];
};

struct CompileData
{ 
    CompileData()
        : accumulated(0u)
        , minimum(0xffffffff)
        , maximum(0u)
        , maxId(InvalidCompileId)
        , count(0u)
    {}

    CompileData(const fastl::string& _name)
        : name(_name)
        , accumulated(0u)
        , minimum(0xffffffff)
        , maximum(0u)
        , maxId(InvalidCompileId)
        , count(0u)
    {}

    fastl::string name; 
    U64           accumulated; 
    U32           minimum; 
    U32           maximum; 
    U32           maxId; //filled by the ScoreProcessor
    U32           count;
};

struct CompileEvent
{ 
    CompileEvent()
        : category(CompileCategory::Invalid)
        , start(0u)
        , duration(0u)
        , nameId(InvalidCompileId)
    {}

    CompileEvent(CompileCategory _category, U32 _start, U32 _duration, const fastl::string& _name)
        : category(_category)
        , start(_start)
        , duration(_duration)
        , name(_name)
        , nameId(InvalidCompileId)
    {}

    fastl::string   name; 
    U32             nameId; //filled by the ScoreProcessor
    U32             start; 
    U32             duration;
    CompileCategory category; 
};

using TCompileIndexSet = fastl::unordered_set<U32>;

struct CompileIncluder
{
    TCompileIndexSet includes;
    TCompileIndexSet units;
};

using TCompileDataDictionary   = fastl::unordered_map<fastl::string,U32>;
using TCompileDatas            = fastl::vector<CompileData>;
using TCompileUnits            = fastl::vector<CompileUnit>;
using TCompileIncluders        = fastl::vector<CompileIncluder>;
using TCompileEvents           = fastl::vector<CompileEvent>;
using TCompileEventTracks      = fastl::vector<TCompileEvents>;

struct ScoreTimeline
{ 
    TCompileEventTracks tracks;
    fastl::string       name;
};

struct ScoreData
{ 
    //exported data
    TCompileUnits     units;
    TCompileDatas     globals[ToUnderlying(CompileCategory::GatherFull)];
    TCompileIncluders includers;

    //helper data
    TCompileDataDictionary globalsDictionary[ToUnderlying(CompileCategory::GatherFull)];
};

