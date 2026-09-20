/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
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
    Foam::wedgeFaePatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    wedgeFaePatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_wedgeFaePatchField_defines_typeName in .C files that
//  define the template statics of wedgeFaePatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_wedgeFaePatchField_defines_typeName)
#  define Foam_wedgeFaePatchField_typeName_API
#else
#  define Foam_wedgeFaePatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_wedgeFaePatchField_H
#define Foam_wedgeFaePatchField_H

#include "faePatchField.H"
#include "wedgeFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class wedgeFaePatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class wedgeFaePatchField
:
    public faePatchField<Type>
{
    typedef wedgeFaePatchField<Type> this_bctype;
    typedef faePatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_wedgeFaePatchField_typeName_API, wedgeFaPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        wedgeFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- Construct from patch, internal field and dictionary
        wedgeFaePatchField
        (
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        wedgeFaePatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, edgeMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        wedgeFaePatchField
        (
            const this_bctype&,
            const DimensionedField<Type, edgeMesh>&
        );

        //- No copy without an internal field
        wedgeFaePatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faePatchField<Type>> clone
        (
            const DimensionedField<Type, edgeMesh>& iF
        ) const
        {
            return faePatchField<Type>::Clone(*this, iF);
        }


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "wedgeFaePatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
