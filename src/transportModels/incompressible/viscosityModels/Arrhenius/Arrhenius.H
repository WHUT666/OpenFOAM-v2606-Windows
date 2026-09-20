/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2017-2019 OpenCFD Ltd.
    Copyright (C) 2026 Keysight Technologies
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
    Foam::viscosityModels::Arrhenius

Description
    Arrhenius type of dependency on a given scalar field name. Most likely
    temperature. The expression is as follow:
    \verbatim
        mu = exp(-alpha_*(T - Talpha_))
    \endverbatim

SourceFiles
    Arrhenius.C

\*---------------------------------------------------------------------------*/

//- Define Foam_Arrhenius_defines_typeName in .C files that
//  define the template statics of Arrhenius (eg via
//  defineTemplateTypeNameAndDebug / *RunTimeSelectionTable)
//  so the declaration is left undecorated there.
#if defined(Foam_Arrhenius_defines_typeName)
#  define Foam_Arrhenius_typeName_API
#else
#  define Foam_Arrhenius_typeName_API incompressibleTransportModels_API
#endif

#ifndef Arrhenius_H
#define Arrhenius_H

#include "viscosityModel.H"
#include "dimensionedScalar.H"
#include "volFields.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace viscosityModels
{

/*---------------------------------------------------------------------------*\
                           Class Arrhenius Declaration
\*---------------------------------------------------------------------------*/

template<class ViscousModel>
class Arrhenius
:
    public ViscousModel
{
    // Private data

        dictionary ArrheniusCoeffs_;

        // Model coefficients
        dimensionedScalar alpha_;
        dimensionedScalar Talpha_;

        //- Field used for as temperature
        word fieldName_;

        //- Auto pointer for scalar field
        autoPtr<volScalarField> field_;

        //- Reference to mesh
        const fvMesh& mesh_;


    // Private Member Functions

        //- Calculate and return the laminar viscosity
        tmp<volScalarField> calcNu(const volScalarField&) const;


public:

    //- Runtime type information
    TypeNameApi(Foam_Arrhenius_typeName_API, "Arrhenius");


    // Constructors

        //- Construct from components
        Arrhenius
        (
            const word& name,
            const dictionary& viscosityProperties,
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    virtual ~Arrhenius() = default;


    // Member Functions

        //- Correct the laminar viscosity
        virtual void correct()
        {
            this->nu_ = ViscousModel::calcNu();

            if (const auto* ptr = mesh_.cfindObject<volScalarField>(fieldName_))
            {
                this->nu_ *= calcNu(*ptr);
            }
        }

        //- Read transportProperties dictionary
        virtual bool read(const dictionary& viscosityProperties);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace viscosityModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
#   include "Arrhenius.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
