
#include "OptionsPanel.h"

#include <wx/sizer.h>
#include <wx/checkbox.h>


#include <assimp.hpp>
#include <aiScene.h>
#include <aiMesh.h>
#include <aiPostProcess.h>

wxWindowID CALC_TANGENT_SPACE_ID = wxNewId();
wxWindowID JOIN_IDENTICAL_VERTICES_ID = wxNewId();
wxWindowID CONVERT_TO_LH_ID = wxNewId();
wxWindowID TRIANGULATE_ID = wxNewId();
wxWindowID REMOVE_COMPONENT_ID = wxNewId();
wxWindowID GENERATE_NORMALS_ID = wxNewId();
wxWindowID GENERATE_SMOOTH_NORMALS_ID = wxNewId();
wxWindowID SPLIT_LARGE_MESHES_ID  = wxNewId();
wxWindowID PRETRANSFORM_VERTICES_ID  = wxNewId();
wxWindowID LIMIT_BONE_WEIGHTS_ID  = wxNewId();
wxWindowID VALIDATE_DATA_STRUCTURE_ID  = wxNewId();
wxWindowID IMPROVE_CACHE_LOCALITY_ID  = wxNewId();
wxWindowID REMOVE_REDUNDANT_MATERIALS_ID  = wxNewId();
wxWindowID FIX_INFACING_NORMALS_ID  = wxNewId();
wxWindowID OPTIMIZE_GRAPH_ID  = wxNewId();
wxWindowID SORT_BY_PRIM_TYPE_ID  = wxNewId();
wxWindowID FIND_DEGENERATES_ID  = wxNewId();
wxWindowID FIND_INVALID_DATA_ID  = wxNewId();
wxWindowID GENERATE_UV_COORDS  = wxNewId();
wxWindowID TRANSFORM_UV_COORDS  = wxNewId();

IMPLEMENT_CLASS(OptionsPanel, wxPanel);

OptionsPanel::OptionsPanel(wxWindow* parent, wxWindowID id /* = wxID_ANY */, const wxPoint& pos /* = wxDefaultPosition */, const wxSize& size /* = wxDeafultSize */, long style /* = wxTAB_TRAVERSAL */, const wxString& name /* =  */)
  : wxPanel(parent, id, pos, size, style, name),
    mOptions( aiProcess_Triangulate | aiProcess_GenNormals )
{
    mSizer = new wxFlexGridSizer( 20, 0, 0, 5 );
    mSizer->AddGrowableCol(0);

    mCheckBox0 = new wxCheckBox( this, CALC_TANGENT_SPACE_ID, "Calculate Tangent Space"  );
    mCheckBox1 = new wxCheckBox( this, JOIN_IDENTICAL_VERTICES_ID,  "Join Identical Vertices" );
    mCheckBox2 = new wxCheckBox( this, CONVERT_TO_LH_ID, "Convert to Left Handed");
    mCheckBox3 = new wxCheckBox( this, TRIANGULATE_ID, "Triangulate");
    mCheckBox4 = new wxCheckBox( this, REMOVE_COMPONENT_ID, "Remove Components");
    mCheckBox5 = new wxCheckBox( this, GENERATE_NORMALS_ID, "Generate Normals");
    mCheckBox6 = new wxCheckBox( this, GENERATE_SMOOTH_NORMALS_ID, "Generate Smooth Normals");
    mCheckBox7 = new wxCheckBox( this, SPLIT_LARGE_MESHES_ID, "Split Large Meshes");
    mCheckBox8 = new wxCheckBox( this, PRETRANSFORM_VERTICES_ID, "Pretransform Vertices");
    mCheckBox9 = new wxCheckBox( this, LIMIT_BONE_WEIGHTS_ID, "Limit Bone Weights");
    mCheckBox10 = new wxCheckBox( this, VALIDATE_DATA_STRUCTURE_ID, "Validate Data Structure");
    mCheckBox11 = new wxCheckBox( this, IMPROVE_CACHE_LOCALITY_ID, "Improve Cache Locality");
    mCheckBox12 = new wxCheckBox( this, REMOVE_REDUNDANT_MATERIALS_ID, "Remove Reduntant Materials");
    mCheckBox13 = new wxCheckBox( this, FIX_INFACING_NORMALS_ID, "Fix InFacing normals");
    mCheckBox14 = new wxCheckBox( this, OPTIMIZE_GRAPH_ID, "Optimize Graph");
    mCheckBox15 = new wxCheckBox( this, SORT_BY_PRIM_TYPE_ID, "Sort by primitive Type");
    mCheckBox16 = new wxCheckBox( this, FIND_DEGENERATES_ID, "Find Degenerates");
    mCheckBox17 = new wxCheckBox( this, FIND_INVALID_DATA_ID, "Find Invalid Data");
    mCheckBox18 = new wxCheckBox( this, GENERATE_UV_COORDS, "Generate UV Coords");
    mCheckBox19 = new wxCheckBox( this, TRANSFORM_UV_COORDS, "Transform UV Coords");

    mSizer->Add(mCheckBox0);
    mSizer->Add(mCheckBox1);
    mSizer->Add(mCheckBox2);
    mSizer->Add(mCheckBox3);
    mSizer->Add(mCheckBox4);
    mSizer->Add(mCheckBox5);
    mSizer->Add(mCheckBox6);
    mSizer->Add(mCheckBox7);
    mSizer->Add(mCheckBox8);
    mSizer->Add(mCheckBox9);
    mSizer->Add(mCheckBox10);
    mSizer->Add(mCheckBox11);
    mSizer->Add(mCheckBox12);
    mSizer->Add(mCheckBox13);
    mSizer->Add(mCheckBox14);
    mSizer->Add(mCheckBox15);
    mSizer->Add(mCheckBox16);
    mSizer->Add(mCheckBox17);
    mSizer->Add(mCheckBox18);
    mSizer->Add(mCheckBox19);

    SetSizer(mSizer);
    Layout();
}

OptionsPanel::~OptionsPanel()
{
}

unsigned int OptionsPanel::getOptions()
{
    return mOptions;
}

void OptionsPanel::setOptions(unsigned int options)
{
    mOptions=options;
}

bool OptionsPanel::TransferDataFromWindow()
{

    mOptions = mCheckBox0->GetValue() ? aiProcess_CalcTangentSpace  : 0;
    mOptions |= mCheckBox1->GetValue() ? aiProcess_JoinIdenticalVertices : 0;
    mOptions |= mCheckBox2->GetValue() ? aiProcess_ConvertToLeftHanded : 0;
    mOptions |= mCheckBox3->GetValue() ? aiProcess_Triangulate : 0;
    mOptions |= mCheckBox4->GetValue() ? aiProcess_RemoveComponent : 0;
    mOptions |= mCheckBox5->GetValue() ? aiProcess_GenNormals : 0;
    mOptions |= mCheckBox6->GetValue() ? aiProcess_GenSmoothNormals : 0;
    mOptions |= mCheckBox7->GetValue() ? aiProcess_SplitLargeMeshes : 0;
    mOptions |= mCheckBox8->GetValue() ? aiProcess_PreTransformVertices : 0;
    mOptions |= mCheckBox9->GetValue() ? aiProcess_LimitBoneWeights : 0;
    mOptions |= mCheckBox10->GetValue() ? aiProcess_ValidateDataStructure : 0;
    mOptions |= mCheckBox11->GetValue() ? aiProcess_ImproveCacheLocality : 0;
    mOptions |= mCheckBox12->GetValue() ? aiProcess_RemoveRedundantMaterials : 0;
    mOptions |= mCheckBox13->GetValue() ? aiProcess_FixInfacingNormals : 0;
    mOptions |= mCheckBox14->GetValue() ? aiProcess_OptimizeGraph : 0;
    mOptions |= mCheckBox15->GetValue() ? aiProcess_SortByPType : 0;
    mOptions |= mCheckBox16->GetValue() ? aiProcess_FindDegenerates : 0;
    mOptions |= mCheckBox17->GetValue() ? aiProcess_FindInvalidData : 0;
    mOptions |= mCheckBox18->GetValue() ? aiProcess_GenUVCoords : 0;
    mOptions |= mCheckBox19->GetValue() ? aiProcess_TransformUVCoords : 0;

    return true;
}                                                                             

bool OptionsPanel::TransferDataToWindow()
{

    mCheckBox0->SetValue( ( mOptions &  aiProcess_CalcTangentSpace           ) != 0);
    mCheckBox1->SetValue( ( mOptions &   aiProcess_JoinIdenticalVertices     ) != 0);
    mCheckBox2->SetValue( ( mOptions &   aiProcess_ConvertToLeftHanded       ) != 0);
    mCheckBox3->SetValue( ( mOptions &   aiProcess_Triangulate                ) != 0);
    mCheckBox4->SetValue( ( mOptions &   aiProcess_RemoveComponent           ) != 0);
    mCheckBox5->SetValue( ( mOptions &   aiProcess_GenNormals                 ) != 0);
    mCheckBox6->SetValue( ( mOptions &   aiProcess_GenSmoothNormals          ) != 0);
    mCheckBox7->SetValue( ( mOptions &   aiProcess_SplitLargeMeshes          ) != 0);
    mCheckBox8->SetValue( ( mOptions &   aiProcess_PreTransformVertices      ) != 0);
    mCheckBox9->SetValue( ( mOptions &   aiProcess_LimitBoneWeights           ) != 0);
    mCheckBox10->SetValue( ( mOptions &  aiProcess_ValidateDataStructure     ) != 0);
    mCheckBox11->SetValue( ( mOptions &  aiProcess_ImproveCacheLocality       ) != 0);
    mCheckBox12->SetValue( ( mOptions &  aiProcess_RemoveRedundantMaterials     ) != 0);
    mCheckBox13->SetValue( ( mOptions &  aiProcess_FixInfacingNormals         ) != 0);
    mCheckBox14->SetValue( ( mOptions &  aiProcess_OptimizeGraph              ) != 0);
    mCheckBox15->SetValue( ( mOptions &  aiProcess_SortByPType                ) != 0);
    mCheckBox16->SetValue( ( mOptions &  aiProcess_FindDegenerates           ) != 0);
    mCheckBox17->SetValue( ( mOptions &  aiProcess_FindInvalidData           ) != 0);
    mCheckBox18->SetValue( ( mOptions &  aiProcess_GenUVCoords                ) != 0);
    mCheckBox19->SetValue( ( mOptions &   aiProcess_TransformUVCoords         ) != 0);

    return true;
}
