/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2016 OpenFOAM Foundation
    Copyright (C) 2019-2026 OpenCFD Ltd.
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
    Foam::genericFaPatchField

Description
    This boundary condition provides a generic version of the \c calculated
    condition, useful as a fallback for handling unknown patch types.  Not
    generally applicable as a user-specified condition.

See also
    Foam::calculatedFaPatchField

SourceFiles
    genericFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_genericFaPatchField_defines_typeName in .C files that
//  define the template statics of genericFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_genericFaPatchField_defines_typeName)
#  define Foam_genericFaPatchField_typeName_API
#else
#  define Foam_genericFaPatchField_typeName_API genericPatchFields_API
#endif

#ifndef Foam_genericFaPatchField_H
#define Foam_genericFaPatchField_H

#include "calculatedFaPatchField.H"
#include "genericPatchFieldBase.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class genericFaPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class genericFaPatchField
:
    public genericPatchFieldBase,
    public calculatedFaPatchField<Type>
{
    typedef genericFaPatchField<Type> this_bctype;
    typedef calculatedFaPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_genericFaPatchField_typeName_API, "generic");


    // Constructors

        //- Construct from patch and internal field
        genericFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch, internal field and dictionary
        genericFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        genericFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Copy construct setting internal field reference
        genericFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without internal field
        genericFaPatchField(const this_bctype& pfld)
        :
            this_bctype(pfld, pfld.internalField())
        {}

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    // Member Functions

        //- Write
        virtual void write(Ostream&) const;


    // Mapping Functions

        //- Map (and resize as needed) from self given a mapping object
        virtual void autoMap(const faPatchFieldMapper&);

        //- Reverse map the given faPatchField onto this faPatchField
        virtual void rmap(const faPatchField<Type>&, const labelList&);


    // Evaluation Functions

        //- Fatal
        virtual tmp<Field<Type>>
        valueInternalCoeffs(const tmp<scalarField>&) const;

        //- Fatal
        virtual tmp<Field<Type>>
        valueBoundaryCoeffs(const tmp<scalarField>&) const;

        //- Fatal
        tmp<Field<Type>> gradientInternalCoeffs() const;

        //- Fatal
        tmp<Field<Type>> gradientBoundaryCoeffs() const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "genericFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
