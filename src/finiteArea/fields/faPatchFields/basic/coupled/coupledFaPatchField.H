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
    Foam::coupledFaPatchField

Description

Author
    Zeljko Tukovic, FMENA
    Hrvoje Jasak, Wikki Ltd.

SourceFiles
    coupledFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_coupledFaPatchField_defines_typeName in .C files that
//  define the template statics of coupledFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_coupledFaPatchField_defines_typeName)
#  define Foam_coupledFaPatchField_typeName_API
#else
#  define Foam_coupledFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_coupledFaPatchField_H
#define Foam_coupledFaPatchField_H

#include "lduInterfaceField.H"
#include "faPatchField.H"
#include "coupledFaPatch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                     Class coupledFaPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class coupledFaPatchField
:
    public lduInterfaceField,
    public faPatchField<Type>
{
    typedef coupledFaPatchField<Type> this_bctype;
    typedef faPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_coupledFaPatchField_typeName_API, coupledFaPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        coupledFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch and internal field and patch field
        coupledFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        coupledFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary& dict,
            //! The "value" entry (default: mandatory)
            IOobjectOption::readOption requireValue = IOobjectOption::MUST_READ
        );

        //- Construct by mapping onto a new patch
        coupledFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        coupledFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        coupledFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>&
        ) const = 0;


    // Member Functions

        // Access

            //- True if derived from coupledFaPatchField
            virtual bool coupled() const { return true; }

            //- Return neighbour field of internal field
            virtual tmp<Field<Type>> patchNeighbourField() const = 0;

            //- Retrieve neighbour coupled field
            virtual void patchNeighbourField(UList<Type>&) const = 0;


        // Evaluation functions

            //- Return patch-normal gradient
            virtual tmp<Field<Type>> snGrad() const;

            //- Retrieve patch-normal gradient
            virtual void snGrad(UList<Type>& result) const;

            //- Initialise the evaluation of the patch field
            virtual void initEvaluate
            (
                const Pstream::commsTypes commsType
            );

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType
            );

            //- Initialise the evaluation of the patch field after a local
            //  operation
            virtual void initEvaluateLocal
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::buffered
            )
            {
                initEvaluate(commsType);
            }

            //- Evaluate the patch field after a local operation (e.g. *=)
            virtual void evaluateLocal
            (
                const Pstream::commsTypes commsType =
                    Pstream::commsTypes::buffered
            )
            {
                evaluate(commsType);
            }

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientBoundaryCoeffs() const;


        // Coupled interface functionality

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                solveScalarField& result,
                const bool add,
                const lduAddressing& lduAddr,
                const label patchId,
                const solveScalarField& psiInternal,
                const scalarField& coeffs,
                const direction,
                const Pstream::commsTypes commsType
            ) const = 0;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                Field<Type>& result,
                const bool add,
                const lduAddressing& lduAddr,
                const label patchId,
                const Field<Type>&,
                const scalarField& coeffs,
                const Pstream::commsTypes commsType
            ) const = 0;


    // Other

        //- Write
        virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "coupledFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
