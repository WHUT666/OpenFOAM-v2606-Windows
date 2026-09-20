/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2018-2023 OpenCFD Ltd.
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

Class
    Foam::IOPtrList

Description
    A PtrList of objects of type \<T\> with automated input and output.

SourceFiles
    IOPtrList.C

\*---------------------------------------------------------------------------*/

//- Define Foam_IOPtrList_defines_typeName in .C files that
//  define the template statics of IOPtrList (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_IOPtrList_defines_typeName)
#  define Foam_IOPtrList_typeName_API
#else
#  define Foam_IOPtrList_typeName_API OpenFOAM_API
#endif

#ifndef Foam_IOPtrList_H
#define Foam_IOPtrList_H

#include "PtrList.H"
#include "regIOobject.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                           Class IOPtrList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class IOPtrList
:
    public regIOobject,
    public PtrList<T>
{
public:

    //- Runtime type information
    TypeNameApi(Foam_IOPtrList_typeName_API, "PtrList");


    // Constructors

        //- Default copy construct
        IOPtrList(const IOPtrList&) = default;

        //- Construct from IOobject
        explicit IOPtrList(const IOobject& io);

        //- Construct from IOobject using given Istream constructor class
        template<class INew>
        IOPtrList(const IOobject& io, const INew& inewt);

        //- Construct from IOobject with given size
        IOPtrList(const IOobject& io, const label len);

        //- Construct from IOobject and a copy of PtrList content
        IOPtrList(const IOobject& io, const PtrList<T>& content);

        //- Construct by transferring the PtrList content
        IOPtrList(const IOobject& io, PtrList<T>&& content);


    // Factory Methods

        //- Read and return contents. The IOobject will not be registered
        static PtrList<T> readContents(const IOobject& io);


    //- Destructor
    virtual ~IOPtrList() = default;


    // Member Functions

        bool writeData(Ostream& os) const;


    // Member Operators

        //- Copy or move assignment of entries
        using PtrList<T>::operator=;

        //- Copy assignment of entries
        void operator=(const IOPtrList<T>& rhs)
        {
            PtrList<T>::operator=(rhs);
        }

        //- Move assignment of entries
        void operator=(IOPtrList<T>&& rhs)
        {
            PtrList<T>::operator=(std::move(static_cast<PtrList<T>&>(rhs)));
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "IOPtrList.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
