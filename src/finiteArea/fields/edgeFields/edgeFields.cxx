#define Foam_GeometricField_defines_typeName
#define Foam_DimensionedField_defines_typeName
#define Foam_GeometricBoundaryField_defines_typeName
/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2016-2017 Wikki Ltd
    Copyright (C) 2026 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/

#include "faMesh.H"
#include "edgeFields.H"
#include "registerSwitch.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{

defineTemplateTypeNameAndDebug(edgeScalarField::Internal, 0);
defineTemplateTypeNameAndDebug(edgeVectorField::Internal, 0);
defineTemplateTypeNameAndDebug(edgeSphericalTensorField::Internal, 0);
defineTemplateTypeNameAndDebug(edgeSymmTensorField::Internal, 0);
defineTemplateTypeNameAndDebug(edgeTensorField::Internal, 0);

defineTemplateTypeNameAndDebug(edgeScalarField, 0);
defineTemplateTypeNameAndDebug(edgeVectorField, 0);
defineTemplateTypeNameAndDebug(edgeSphericalTensorField, 0);
defineTemplateTypeNameAndDebug(edgeSymmTensorField, 0);
defineTemplateTypeNameAndDebug(edgeTensorField, 0);

defineTemplateDebugSwitchWithName
(
    edgeScalarField::Boundary,
    "edgeScalarField::Boundary",
    0
);
defineTemplateDebugSwitchWithName
(
    edgeVectorField::Boundary,
    "edgeVectorField::Boundary",
    0
);
defineTemplateDebugSwitchWithName
(
    edgeSphericalTensorField::Boundary,
    "edgeSphericalTensorField::Boundary",
    0
);
defineTemplateDebugSwitchWithName
(
    edgeSymmTensorField::Boundary,
    "edgeSymmTensorField::Boundary",
    0
);
defineTemplateDebugSwitchWithName
(
    edgeTensorField::Boundary,
    "edgeTensorField::Boundary",
    0
);

} // End namespace Foam


// * * * * * * * * * * * * * * * * Global Data * * * * * * * * * * * * * * * //

// Note hard-coded values are more reliable than other alternatives

const Foam::wordList Foam::fieldTypes::edge
({
    "edgeScalarField",
    "edgeVectorField",
    // Probably don't need anything beyond scalar...
    "edgeSphericalTensorField",
    "edgeSymmTensorField",
    "edgeTensorField"
});


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

bool Foam::fieldTypes::is_edge(const word& clsName)
{
    return
    (
        clsName.starts_with("edge") && clsName.ends_with("Field")
     && Foam::fieldTypes::edge.contains(clsName)
    );
}


// ************************************************************************* //
