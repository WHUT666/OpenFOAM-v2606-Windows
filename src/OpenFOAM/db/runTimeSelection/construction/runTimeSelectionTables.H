/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019-2022 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Description
    Macros to ease declaration of run-time selection tables.

    declareRunTimeSelectionTable is used to create a run-time selection table
    for a base-class which holds constructor pointers on the table.

    declareRunTimeNewSelectionTable is used to create a run-time selection
    table for a derived-class which holds "New" pointers on the table.

\*---------------------------------------------------------------------------*/

#include "token.H"

#ifndef Foam_runTimeSelectionTables_H
#define Foam_runTimeSelectionTables_H

#include <memory>   // For std::unique_ptr
#include <utility>  // For std::pair

#include "autoPtr.H"
#include "HashTable.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Common infrastructure

// Not used directly: declare run-time selection (variables and methods)
//  The api argument is a declspec specifier for Windows DLL builds (or empty)
#define declareRunTimeSelectionTableBase(api,returnType,prefix,argList)        \
                                                                               \
    /* Function pointer type: construct from argList */                        \
    typedef returnType (*prefix##Ptr)argList;                                  \
                                                                               \
    /* Function table type: construct from argList */                          \
    typedef ::Foam::HashTable                                                  \
    <                                                                          \
        prefix##Ptr,                                                           \
        ::Foam::word,                                                          \
        ::Foam::Hash<::Foam::word>                                             \
    > prefix##TableType;                                                       \
                                                                               \
    /* Lookup aliases for constructors */                                      \
    typedef ::Foam::HashTable                                                  \
    <                                                                          \
        std::pair<::Foam::word,int>,                                           \
        ::Foam::word,                                                          \
        ::Foam::Hash<::Foam::word>                                             \
    > prefix##CompatTableType;                                                 \
                                                                               \
    /* Table singleton (storage) */                                            \
    static api prefix##TableType* prefix##TablePtr_;                           \
                                                                               \
    /* Lookup aliases singleton (storage) */                                   \
    static api std::unique_ptr<prefix##CompatTableType> prefix##CompatTablePtr_; \
                                                                               \
    /* Aliases singleton (access) */                                           \
    static prefix##CompatTableType& prefix##CompatTable();                     \
                                                                               \
    /* Table construct/destruct helper */                                      \
    static void prefix##TablePtr_construct(bool load);                         \
                                                                               \
    /* Table insert (with duplicate check), set and erase helpers. */          \
    /* These encapsulate all access to the table singleton so that         */  \
    /* consumers only ever reference member functions, never the data      */  \
    /* member directly (required for reliable dllimport on Windows).       */  \
    static void prefix##TableInsert(const ::Foam::word& k, prefix##Ptr f);     \
    static void prefix##TableSet(const ::Foam::word& k, prefix##Ptr f);        \
    static void prefix##TableErase(const ::Foam::word& k);                     \
                                                                               \
    /* Lookup function pointer from singleton, nullptr if not found */         \
    static prefix##Ptr prefix##Table(const ::Foam::word& k)


// Not used directly: storage and helper methods for runtime tables
// - uses automatic cleanup for compat table (issue #2314)
#define defineRunTimeSelectionTableBase(baseType,prefix,Tspecialize)           \
                                                                               \
    /* Define table singleton (storage) */                                     \
    Tspecialize prefix##TableType* prefix##TablePtr_(nullptr);                 \
                                                                               \
    /* Define aliases singleton (storage) */                                   \
    Tspecialize                                                                \
    std::unique_ptr<prefix##CompatTableType> prefix##CompatTablePtr_(nullptr); \
                                                                               \
    /* Define table singleton (access) for aliases */                          \
    Tspecialize prefix##CompatTableType& prefix##CompatTable()                 \
    {                                                                          \
        if (!prefix##CompatTablePtr_)                                          \
        {                                                                      \
            prefix##CompatTablePtr_.reset(new prefix##CompatTableType(16));    \
        }                                                                      \
        return *(prefix##CompatTablePtr_);                                     \
    }                                                                          \
                                                                               \
    /* Table construct/destruct helper */                                      \
    Tspecialize void prefix##TablePtr_construct(bool load)                     \
    {                                                                          \
        static bool constructed = false;                                       \
        if (load)                                                              \
        {                                                                      \
            if (!constructed)                                                  \
            {                                                                  \
                prefix##TablePtr_ = new prefix##TableType;                     \
                constructed = true;                                            \
            }                                                                  \
        }                                                                      \
        else if (prefix##TablePtr_)                                            \
        {                                                                      \
            delete prefix##TablePtr_;                                          \
            prefix##TablePtr_ = nullptr;                                       \
        }                                                                      \
    }                                                                          \
                                                                               \
    /* Insert entry, with duplicate check */                                   \
    Tspecialize void prefix##TableInsert(const ::Foam::word& k, prefix##Ptr f) \
    {                                                                          \
        if (prefix##TablePtr_)                                                 \
        {                                                                      \
            if                                                               \
            (                                                                \
                !prefix##TablePtr_->insert(k, f)                             \
             && (*prefix##TablePtr_)[k] != f                                 \
            )                                                                \
            {                                                                \
                std::cerr                                                    \
                    << "Duplicate entry " << k                               \
                    << " in runtime table " << #baseType << std::endl;       \
                ::Foam::error::safePrintStack(std::cerr);                    \
            }                                                                \
        }                                                                      \
    }                                                                          \
                                                                               \
    /* Set (insert or overwrite) entry */                                      \
    Tspecialize void prefix##TableSet(const ::Foam::word& k, prefix##Ptr f)    \
    {                                                                          \
        if (prefix##TablePtr_)                                                 \
        {                                                                      \
            prefix##TablePtr_->set(k, f);                                      \
        }                                                                      \
    }                                                                          \
                                                                               \
    /* Erase entry */                                                          \
    Tspecialize void prefix##TableErase(const ::Foam::word& k)                 \
    {                                                                          \
        if (prefix##TablePtr_)                                                 \
        {                                                                      \
            prefix##TablePtr_->erase(k);                                       \
        }                                                                      \
    }                                                                          \
                                                                               \
    /* Define lookup function pointer (singleton) */                           \
    Tspecialize prefix##Ptr prefix##Table(const ::Foam::word& k)               \
    {                                                                          \
        if (prefix##TablePtr_)                                                 \
        {                                                                      \
            const auto& tbl = *prefix##TablePtr_;                              \
            auto iter = tbl.cfind(k);                                          \
            if (!iter.good() && prefix##CompatTablePtr_)                       \
            {                                                                  \
                const auto altIter = prefix##CompatTablePtr_->cfind(k);        \
                if (altIter.good())                                            \
                {                                                              \
                    const auto& alt = altIter.val();  /* <word,int> */         \
                    iter = tbl.cfind(alt.first);                               \
                    if (::Foam::error::warnAboutAge(alt.second))               \
                    {                                                          \
                        std::cerr                                              \
                            << "Using [v" << alt.second << "] '" << k          \
                            << "' instead of '" << alt.first                   \
                            << "' in selection table: "                        \
                            << #baseType << '\n' << std::endl;                 \
                        ::Foam::error::warnAboutAge("lookup", alt.second);     \
                    }                                                          \
                }                                                              \
            }                                                                  \
            if (iter.good())                                                   \
            {                                                                  \
                return iter.val();                                             \
            }                                                                  \
        }                                                                      \
        return nullptr;                                                        \
    }


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//
// Declaration Macros
//

//- Declare a run-time selection (variables and adder classes)
//  The ptrWrapper is typically a Foam::autoPtr or a Foam::tmp container.
#define declareRunTimeSelectionTable\
(ptrWrapper,baseType,argNames,argList,parList)                                 \
                                                                               \
    declareRunTimeSelectionTableApi(                                           \
        /*no-api*/, ptrWrapper,baseType,argNames,argList,parList)

//- Same as declareRunTimeSelectionTable, but the table singletons carry an
//  explicit import/export specifier (Windows DLL builds).
#define declareRunTimeSelectionTableApi\
(api,ptrWrapper,baseType,argNames,argList,parList)                             \
                                                                               \
    declareRunTimeSelectionTableBase(                                          \
        api, ptrWrapper<baseType>, argNames##Constructor, argList);            \
                                                                               \
    /* Helper to add compatibility/alias for runtime selection table */        \
    template<class baseType##Type>                                             \
    struct addAlias##argNames##ConstructorToTable                              \
    {                                                                          \
        explicit addAlias##argNames##ConstructorToTable                        \
        (                                                                      \
            const ::Foam::word& k,                                             \
            const ::Foam::word& alias,                                         \
            const int ver                                                      \
        )                                                                      \
        {                                                                      \
            argNames##ConstructorCompatTable()                                 \
                .set(alias, std::pair<::Foam::word,int>(k,ver));               \
        }                                                                      \
    };                                                                         \
                                                                               \
    /* Helper to add constructor from argList to table */                      \
    template<class baseType##Type>                                             \
    struct add##argNames##ConstructorToTable                                   \
    {                                                                          \
        static ptrWrapper<baseType> New argList                                \
        {                                                                      \
            return ptrWrapper<baseType>(new baseType##Type parList);           \
        }                                                                      \
                                                                               \
        template<class baseType##Type##Default_ = baseType##Type>  \
        explicit add##argNames##ConstructorToTable                             \
        (                                                                      \
            const ::Foam::word& k = baseType##Type##Default_::typeName                   \
        )                                                                      \
        {                                                                      \
            argNames##ConstructorTablePtr_construct(true);                     \
            argNames##ConstructorTableInsert(k, New);                          \
        }                                                                      \
                                                                               \
        ~add##argNames##ConstructorToTable()                                   \
        {                                                                      \
            argNames##ConstructorTablePtr_construct(false);                    \
        }                                                                      \
                                                                               \
        add##argNames##ConstructorToTable                                      \
            (const add##argNames##ConstructorToTable&) = delete;               \
                                                                               \
        void operator=                                                         \
            (const add##argNames##ConstructorToTable&) = delete;               \
    };                                                                         \
                                                                               \
    /* Helper to add constructor from argList to table */                      \
    /* Remove only the entry (not the table) upon destruction */               \
    template<class baseType##Type>                                             \
    struct addRemovable##argNames##ConstructorToTable                          \
    {                                                                          \
        const ::Foam::word name;  /* Lookup name for later removal */          \
                                                                               \
        static ptrWrapper<baseType> New argList                                \
        {                                                                      \
            return ptrWrapper<baseType>(new baseType##Type parList);           \
        }                                                                      \
                                                                               \
        template<class baseType##Type##Default_ = baseType##Type>  \
        explicit addRemovable##argNames##ConstructorToTable                    \
        (                                                                      \
            const ::Foam::word& k = baseType##Type##Default_::typeName                   \
        )                                                                      \
        :                                                                      \
            name(k)                                                            \
        {                                                                      \
            argNames##ConstructorTablePtr_construct(true);                     \
            argNames##ConstructorTableSet(k, New);                             \
        }                                                                      \
                                                                               \
        ~addRemovable##argNames##ConstructorToTable()                          \
        {                                                                      \
            argNames##ConstructorTableErase(name);                             \
        }                                                                      \
                                                                               \
        addRemovable##argNames##ConstructorToTable                             \
            (const addRemovable##argNames##ConstructorToTable&) = delete;      \
                                                                               \
        void operator=                                                         \
            (const addRemovable##argNames##ConstructorToTable&) = delete;      \
    };


//- Declare a run-time selection for derived classes
#define declareRunTimeNewSelectionTable\
(ptrWrapper,baseType,argNames,argList,parList)                                 \
                                                                               \
    declareRunTimeNewSelectionTableApi(                                        \
        /*no-api*/, ptrWrapper,baseType,argNames,argList,parList)

//- Same as declareRunTimeNewSelectionTable, but the table singletons carry an
//  explicit import/export specifier (Windows DLL builds).
#define declareRunTimeNewSelectionTableApi\
(api,ptrWrapper,baseType,argNames,argList,parList)                             \
                                                                               \
    declareRunTimeSelectionTableBase(                                          \
        api, ptrWrapper<baseType>,argNames##Constructor,argList);              \
                                                                               \
    /* Helper to add compatibility/alias for runtime selection table */        \
    template<class baseType##Type>                                             \
    struct addAlias##argNames##ConstructorToTable                              \
    {                                                                          \
        explicit addAlias##argNames##ConstructorToTable                        \
        (                                                                      \
            const ::Foam::word& k,                                             \
            const ::Foam::word& alias,                                         \
            const int ver                                                      \
        )                                                                      \
        {                                                                      \
            argNames##ConstructorCompatTable()                                 \
                .set(alias, std::pair<::Foam::word,int>(k,ver));               \
        }                                                                      \
    };                                                                         \
                                                                               \
    /* Helper to add constructor from argList to table */                      \
    template<class baseType##Type>                                             \
    struct add##argNames##ConstructorToTable                                   \
    {                                                                          \
        static ptrWrapper<baseType> New##baseType argList                      \
        {                                                                      \
            return ptrWrapper<baseType>(baseType##Type::New parList.ptr());    \
        }                                                                      \
                                                                               \
        template<class baseType##Type##Default_ = baseType##Type>  \
        explicit add##argNames##ConstructorToTable                             \
        (                                                                      \
            const ::Foam::word& k = baseType##Type##Default_::typeName                   \
        )                                                                      \
        {                                                                      \
            argNames##ConstructorTablePtr_construct(true);                     \
            argNames##ConstructorTableInsert(k, New##baseType);                \
        }                                                                      \
                                                                               \
        ~add##argNames##ConstructorToTable()                                   \
        {                                                                      \
            argNames##ConstructorTablePtr_construct(false);                    \
        }                                                                      \
                                                                               \
        add##argNames##ConstructorToTable                                      \
            (const add##argNames##ConstructorToTable&) = delete;               \
                                                                               \
        void operator=                                                         \
            (const add##argNames##ConstructorToTable&) = delete;               \
    };                                                                         \
                                                                               \
    /* Helper to add constructor from argList to table */                      \
    template<class baseType##Type>                                             \
    struct addRemovable##argNames##ConstructorToTable                          \
    {                                                                          \
        const ::Foam::word name;  /* Retain name for later removal */          \
                                                                               \
        static ptrWrapper<baseType> New##baseType argList                      \
        {                                                                      \
            return ptrWrapper<baseType>(baseType##Type::New parList.ptr());    \
        }                                                                      \
                                                                               \
        template<class baseType##Type##Default_ = baseType##Type>  \
        explicit addRemovable##argNames##ConstructorToTable                    \
        (                                                                      \
            const ::Foam::word& k = baseType##Type##Default_::typeName                   \
        )                                                                      \
        :                                                                      \
            name(k)                                                            \
        {                                                                      \
            argNames##ConstructorTablePtr_construct(true);                     \
            argNames##ConstructorTableSet(k, New##baseType);                   \
        }                                                                      \
                                                                               \
        ~addRemovable##argNames##ConstructorToTable()                          \
        {                                                                      \
            argNames##ConstructorTableErase(name);                             \
        }                                                                      \
                                                                               \
        addRemovable##argNames##ConstructorToTable                             \
            (const addRemovable##argNames##ConstructorToTable&) = delete;      \
                                                                               \
        void operator=                                                         \
            (const addRemovable##argNames##ConstructorToTable&) = delete;      \
    };


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//
// Definition Macros
//

//- Define run-time selection table
#define defineRunTimeSelectionTable(baseType,argNames)                         \
                                                                               \
    defineRunTimeSelectionTableBase(                                           \
        baseType,baseType::argNames##Constructor,)


//- Define run-time selection table for template classes
//  use when baseType doesn't need a template argument (eg, is a typedef)
#define defineTemplateRunTimeSelectionTable(baseType,argNames)                 \
                                                                               \
    defineRunTimeSelectionTableBase(                                           \
        baseType,baseType::argNames##Constructor,template<>)


//- Define run-time selection table for template classes
//  use when baseType requires the Targ template argument
#define defineTemplatedRunTimeSelectionTable(baseType,argNames,Targ)           \
                                                                               \
    defineRunTimeSelectionTableBase(                                           \
        baseType,baseType<Targ>::argNames##Constructor,template<>)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
