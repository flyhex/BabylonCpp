#include <babylon/materials/node/blocks/fresnel_block.h>

#include <babylon/core/string.h>
#include <babylon/materials/node/input/input_block.h>
#include <babylon/materials/node/node_material_build_state.h>
#include <babylon/materials/node/node_material_connection_point.h>

namespace BABYLON {

FresnelBlock::FresnelBlock(const std::string& iName)
    : NodeMaterialBlock{iName, NodeMaterialBlockTargets::Neutral}
    , worldNormal{this, &FresnelBlock::get_worldNormal}
    , viewDirection{this, &FresnelBlock::get_viewDirection}
    , bias{this, &FresnelBlock::get_bias}
    , power{this, &FresnelBlock::get_power}
    , fresnel{this, &FresnelBlock::get_fresnel}
{
  registerInput("worldNormal", NodeMaterialBlockConnectionPointTypes::Vector4);
  registerInput("viewDirection",
                NodeMaterialBlockConnectionPointTypes::Vector3);
  registerInput("bias", NodeMaterialBlockConnectionPointTypes::Float);
  registerInput("power", NodeMaterialBlockConnectionPointTypes::Float);

  registerOutput("fresnel", NodeMaterialBlockConnectionPointTypes::Float);
}

FresnelBlock::~FresnelBlock()
{
}

const std::string FresnelBlock::getClassName() const
{
  return "FresnelBlock";
}

NodeMaterialConnectionPointPtr& FresnelBlock::get_worldNormal()
{
  return _inputs[0];
}

NodeMaterialConnectionPointPtr& FresnelBlock::get_viewDirection()
{
  return _inputs[1];
}

NodeMaterialConnectionPointPtr& FresnelBlock::get_bias()
{
  return _outputs[2];
}

NodeMaterialConnectionPointPtr& FresnelBlock::get_power()
{
  return _inputs[3];
}

NodeMaterialConnectionPointPtr& FresnelBlock::get_fresnel()
{
  return _outputs[0];
}

void FresnelBlock::autoConfigure(const NodeMaterialPtr& material)
{
  if (!bias()->isConnected()) {
    auto biasInput   = InputBlock::New("bias");
    biasInput->value = 0.f;
    biasInput->output()->connectTo(bias);
  }

  if (!power()->isConnected()) {
    auto powerInput   = InputBlock::New("power");
    powerInput->value = 1.f;
    powerInput->output()->connectTo(power);
  }
}

FresnelBlock& FresnelBlock::_buildBlock(NodeMaterialBuildState& state)
{
  NodeMaterialBlock::_buildBlock(state);

  auto comments = String::printf("//%s", name.c_str());

  state._emitFunctionFromInclude("fresnelFunction", comments,
                                 {.removeIfDef = true});

  state.compilationString
    += _declareOutput(fresnel, state)
       + String::printf(" = computeFresnelTerm(%s, %s, %s, %s);\r\n",
                        viewDirection()->associatedVariableName().c_str(),
                        worldNormal()->associatedVariableName().c_str(),
                        bias()->associatedVariableName().c_str(),
                        power()->associatedVariableName().c_str());

  return *this;
}

} // end of namespace BABYLON