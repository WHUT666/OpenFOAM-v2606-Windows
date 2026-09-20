/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2019-2025 OpenCFD Ltd.
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
    Foam::emptyFaPatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    emptyFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_emptyFaPatchField_defines_typeName in .C files that
//  define the template statics of emptyFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_emptyFaPatchField_defines_typeName)
#  define Foam_emptyFaPatchField_typeName_API
#else
#  define Foam_emptyFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_emptyFaPatchField_H
#define Foam_emptyFaPatchField_H

#include "faPatchField.H"
#include "emptyFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class emptyFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class emptyFaPatchField
:
    public faPatchField<Type>
{
    typedef emptyFaPatchField<Type> this_bctype;
    typedef faPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_emptyFaPatchField_typeName_API, emptyFaPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        emptyFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch, internal field and dictionary.
        //- The "value" entry is NO_READ.
        emptyFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        emptyFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        emptyFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        emptyFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~emptyFaPatchField() = default;


    // Member Functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const faPatchFieldMapper&
            )
            {}

            //- Reverse map the given faPatchField onto this faPatchField
            virtual void rmap
            (
                const faPatchField<Type>&,
                const labelList&
            )
            {}


        // Evaluation Functions

            //- Update the coefficients associated with the patch field
            //  This only checks to see the case is actually 1D or 2D
            //  for which this boundary condition is valid
            void updateCoeffs();


            //- Return the matrix diagonal coefficients corresponding to the
            //- evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const
            {
                return tmp<Field<Type>>::New();
            }

            //- Return the matrix source coefficients corresponding to the
            //- evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const
            {
                return tmp<Field<Type>>::New();
            }

            //- Return the matrix diagonal coefficients corresponding to the
            //- evaluation of the gradient of this patchField
            tmp<Field<Type>> gradientInternalCoeffs() const
            {
                return tmp<Field<Type>>::New();
            }

            //- Return the matrix source coefficients corresponding to the
            //- evaluation of the gradient of this patchField
            tmp<Field<Type>> gradientBoundaryCoeffs() const
            {
                return tmp<Field<Type>>::New();
            }


    // Contiguous storage

        //- Retrieve patch-normal gradient [contiguous storage].
        //- Placeholder value is zero (treated like zero-gradient).
        virtual void snGrad(UList<Type>& result) const
        {
            // Treat like zero-gradient
            result = Foam::zero{};
        }


    // Member Functions

        //- Write without "value" entry!
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment (TBD: could also be ignore assignment)
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "emptyFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
