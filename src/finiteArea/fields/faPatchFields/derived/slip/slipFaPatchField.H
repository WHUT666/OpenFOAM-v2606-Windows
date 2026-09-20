/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2019 OpenCFD Ltd.
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
    Foam::slipFaPatchField

Description
    Foam::slipFaPatchField

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

\*---------------------------------------------------------------------------*/

//- Define Foam_slipFaPatchField_defines_typeName in .C files that
//  define the template statics of slipFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_slipFaPatchField_defines_typeName)
#  define Foam_slipFaPatchField_typeName_API
#else
#  define Foam_slipFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_slipFaPatchField_H
#define Foam_slipFaPatchField_H

#include "basicSymmetryFaPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                         Class slipFaPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class slipFaPatchField
:
    public basicSymmetryFaPatchField<Type>
{
    typedef slipFaPatchField<Type> this_bctype;
    typedef basicSymmetryFaPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_slipFaPatchField_typeName_API, "slip");


    // Constructors

        //- Construct from patch and internal field
        slipFaPatchField
        (
            const faPatch& p,
            const DimensionedField<Type, areaMesh>& iF
        )
        :
            parent_bctype(p, iF)
        {}

        //- Construct from patch, internal field and dictionary
        //  The "value" entry: NO_READ
        slipFaPatchField
        (
            const faPatch& p,
            const DimensionedField<Type, areaMesh>& iF,
            const dictionary& dict
        )
        :
            parent_bctype(p, iF, dict)
        {}

        //- Construct by mapping onto a new patch
        slipFaPatchField
        (
            const slipFaPatchField<Type>& ptf,
            const faPatch& p,
            const DimensionedField<Type, areaMesh>& iF,
            const faPatchFieldMapper& mapper
        )
        :
            parent_bctype(ptf, p, iF, mapper)
        {}

        //- Copy construct with internal field reference
        slipFaPatchField
        (
            const slipFaPatchField<Type>& ptf,
            const DimensionedField<Type, areaMesh>& iF
        )
        :
            parent_bctype(ptf, iF)
        {}

        //- No copy without an internal field
        slipFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~slipFaPatchField() = default;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
