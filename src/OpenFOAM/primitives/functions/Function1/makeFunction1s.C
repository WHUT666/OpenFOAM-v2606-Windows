/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
    Copyright (C) 2020-2021 OpenCFD Ltd.
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

#include "CodedFunction1.H"
#include "Constant.H"
#include "Uniform.H"
#include "ZeroConstant.H"
#include "OneConstant.H"
#include "NoneFunction1.H"
#include "PolynomialEntry.H"
#include "Sine.H"
#include "Cosine.H"
#include "Square.H"
#include "CSV.H"
#include "Table.H"
#include "TableFile.H"
#include "Scale.H"
#include "InputValueMapper.H"
#include "FunctionObjectTrigger.H"
#include "FunctionObjectValue.H"
#include "fieldTypes.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeFunction1s(Type)                                                   \
    makeFunction1(Type);                                                       \
    makeFunction1Type(CodedFunction1, Type);                                   \
    makeFunction1Type(Constant, Type);                                         \
    makeFunction1Type(Uniform, Type);                                          \
    makeFunction1Type(None, Type);                                             \
    makeFunction1Type(ZeroConstant, Type);                                     \
    makeFunction1Type(OneConstant, Type);                                      \
    makeFunction1Type(Polynomial, Type);                                       \
    makeFunction1Type(Cosine, Type);                                           \
    makeFunction1Type(Sine, Type);                                             \
    makeFunction1Type(Square, Type);                                           \
    makeFunction1Type(CSV, Type);                                              \
    makeFunction1Type(Table, Type);                                            \
    makeFunction1Type(TableFile, Type);                                        \
    makeFunction1Type(Scale, Type);                                            \
    makeFunction1Type(InputValueMapper, Type);                                 \
    makeFunction1Type(FunctionObjectValue, Type);

#define makeFieldFunction1s(Type)                                              \
    makeFunction1(Type);                                                       \
    makeFunction1Type(Constant, Type);                                         \
    makeFunction1Type(Uniform, Type);                                          \
    makeFunction1Type(Table, Type);                                            \
    makeFunction1Type(TableFile, Type);                                        \

namespace Foam
{
    makeFunction1(label);
    makeFunction1Type(Constant, label);
    makeFunction1Type(None, label);

    makeFunction1Type(FunctionObjectTrigger, label);
    makeFunction1Type(FunctionObjectTrigger, scalar);
    // Only (label/scalar) makes sense for triggers

    makeFunction1s(scalar);
    makeFunction1s(vector);
    makeFunction1s(sphericalTensor);
    makeFunction1s(symmTensor);
    makeFunction1s(tensor);

    makeFieldFunction1s(scalarField);

    OpenFOAM_TEMPLATE_EXPORT(Function1<Foam::label>);
    OpenFOAM_TEMPLATE_EXPORT(Function1<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1<Foam::scalarField>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Constant<Foam::label>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Constant<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Constant<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Constant<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Constant<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Constant<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Constant<Foam::scalarField>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::None<Foam::label>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::None<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::None<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::None<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::None<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::None<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::FunctionObjectTrigger<Foam::label>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::FunctionObjectTrigger<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CodedFunction1<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CodedFunction1<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CodedFunction1<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CodedFunction1<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CodedFunction1<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::ZeroConstant<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::ZeroConstant<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::ZeroConstant<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::ZeroConstant<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::ZeroConstant<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::OneConstant<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::OneConstant<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::OneConstant<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::OneConstant<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::OneConstant<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Polynomial<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Polynomial<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Polynomial<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Polynomial<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Polynomial<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Cosine<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Cosine<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Cosine<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Cosine<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Cosine<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Sine<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Sine<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Sine<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Sine<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Sine<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Square<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Square<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Square<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Square<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Square<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CSV<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CSV<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CSV<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CSV<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::CSV<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Scale<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Scale<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Scale<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Scale<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Scale<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::InputValueMapper<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::InputValueMapper<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::InputValueMapper<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::InputValueMapper<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::InputValueMapper<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::FunctionObjectValue<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::FunctionObjectValue<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::FunctionObjectValue<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::FunctionObjectValue<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::FunctionObjectValue<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Uniform<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Uniform<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Uniform<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Uniform<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Uniform<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Uniform<Foam::scalarField>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Table<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Table<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Table<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Table<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Table<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::Table<Foam::scalarField>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::TableFile<Foam::scalar>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::TableFile<Foam::vector>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::TableFile<Foam::sphericalTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::TableFile<Foam::symmTensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::TableFile<Foam::tensor>);
    OpenFOAM_TEMPLATE_EXPORT(Function1Types::TableFile<Foam::scalarField>);


}


// ************************************************************************* //
