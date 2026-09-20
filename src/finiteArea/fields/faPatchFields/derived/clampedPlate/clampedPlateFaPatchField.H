/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 OpenCFD Ltd.
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
    Foam::clampedPlateFaPatchField

Description
    This BC provides a clamped BC. It sets zero fixed value and zeroGradient.

Usage
    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        // Mandatory entries (unmodifiable)
        type                clampedPlate;

        // Mandatory/Optional (inherited) entries
        ...
    }
    \endverbatim

    where the entries mean:
    \table
      Property | Description               | Type  | Reqd | Dflt
      type     | Type name: clampedPlate   | word  | yes  | -
    \endtable

    The inherited entries are elaborated in:
      - \link faPatchFields.H \endlink

SourceFiles
    clampedPlateFaPatchField.txx

\*---------------------------------------------------------------------------*/

//- Define Foam_clampedPlateFaPatchField_defines_typeName in .C files that
//  define the template statics of clampedPlateFaPatchField (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_clampedPlateFaPatchField_defines_typeName)
#  define Foam_clampedPlateFaPatchField_typeName_API
#else
#  define Foam_clampedPlateFaPatchField_typeName_API finiteArea_API
#endif

#ifndef Foam_clampedPlateFaPatchField_H
#define Foam_clampedPlateFaPatchField_H

#include "faPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class clampedPlateFaPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class clampedPlateFaPatchField
:
    public faPatchField<Type>
{
    typedef clampedPlateFaPatchField<Type> this_bctype;
    typedef faPatchField<Type> parent_bctype;

public:

    //- Runtime type information
    TypeNameApi(Foam_clampedPlateFaPatchField_typeName_API, "clampedPlate");


    // Constructors

        //- Construct from patch and internal field
        clampedPlateFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&
        );

        //- Construct from patch, internal field and dictionary
        //  The "value" entry: NO_READ
        clampedPlateFaPatchField
        (
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const dictionary&
        );

        //- Construct by mapping onto a new patch
        clampedPlateFaPatchField
        (
            const this_bctype&,
            const faPatch&,
            const DimensionedField<Type, areaMesh>&,
            const faPatchFieldMapper&
        );

        //- Construct as copy setting internal field reference
        clampedPlateFaPatchField
        (
            const this_bctype&,
            const DimensionedField<Type, areaMesh>&
        );

        //- No copy without an internal field
        clampedPlateFaPatchField(const this_bctype&) = delete;

        //- Clone with an internal field reference
        virtual tmp<faPatchField<Type>> clone
        (
            const DimensionedField<Type, areaMesh>& iF
        ) const
        {
            return faPatchField<Type>::Clone(*this, iF);
        }


    //- Destructor
    virtual ~clampedPlateFaPatchField() = default;


    // Member Functions

        // Evaluation

            //- Return gradient at boundary
            virtual tmp<Field<Type>> snGrad() const
            {
                return tmp<Field<Type>>::New(this->size(), Foam::zero{});
            }

            //- Retrieve patch-normal gradient [contiguous storage]
            virtual void snGrad(UList<Type>& result) const
            {
                result = Foam::zero{};
            }

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::commsTypes::buffered
            );

            //- Return the matrix diagonal coefficients corresponding to the
            //- evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //- evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type>> valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //- evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //- evaluation of the gradient of this patchField
            virtual tmp<Field<Type>> gradientBoundaryCoeffs() const;


        // IO

            //- Write includes "value" entry
            virtual void write(Ostream&) const;


    // Member Operators

        //- Inherit assignment
        using parent_bctype::operator=;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "clampedPlateFaPatchField.txx"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
