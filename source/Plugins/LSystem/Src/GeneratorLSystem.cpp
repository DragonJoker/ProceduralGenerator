#include "LSystem/GeneratorLSystem.h"
#include <LibGenerator/Random.h>

#ifdef __WXMSW__
#	include<wx/msw/msvcrt.h>
#endif

using namespace ProceduralTextures;
class LSystem {

  int steps = 0;

  String axiom;
  String rule;
  String production;

  float startLength;
  float drawLength;
  float theta;

  int generations;

  LSystem() {

    axiom = "F";
    rule = "F+F-F";
    startLength = 90.0;
    theta = radians(120.0);
    reset();
  }

  void reset() {
    production = axiom;
    drawLength = startLength;
    generations = 0;
  }

  int getAge() {
    return generations;
  }
  
  void render() {
    translate(width/2, height/2);
    steps += 5;          
    if (steps > production.length()) {
      steps = production.length();
    }
    for (int i = 0; i < steps; i++) {
      char step = production.charAt(i);
      if (step == 'F') {
        rect(0, 0, -drawLength, -drawLength);
        noFill();
        translate(0, -drawLength);
      } 
      else if (step == '+') {
        rotate(theta);
      } 
      else if (step == '-') {
        rotate(-theta);
      } 
      else if (step == '[') {
        pushMatrix();
      } 
      else if (step == ']') {
        popMatrix();            
      }
    }
  }
  
  void simulate(int gen) {
    while (getAge() < gen) {
      production = iterate(production, rule);
    }
  }

  String iterate(String prod_, String rule_) {
    drawLength = drawLength * 0.6;
    generations++;
    String newProduction = prod_;          
    newProduction = newProduction.replaceAll("F", rule_);
    return newProduction;
  }
}

//*************************************************************************************************

LSystem::LSystemDefinition :: LSystemDefinition()
	:	m_pStart( NULL)
{
}

LSystem::LSystemDefinition :: ~LSystemDefinition()
{
}

//*************************************************************************************************

LSystem::Thread :: Thread( ProceduralGenerator * p_pParent, size_t p_uiIndex, int iWidth, int iTop, int iBottom, int iTotalHeight, const UbPixel & p_pxColour)
	:	ProceduralGenerator::Thread( p_pParent, p_uiIndex, iWidth, iTop, iBottom, iTotalHeight, p_pxColour)
{
}

LSystem::Thread :: ~Thread()
{
}

void LSystem::Thread :: Step()
{
}

void LSystem::Thread :: SetSystem( RuleVariableMap * p_pRules, ConstantArray * p_pConstants, Start ** p_ppStart, VariableList * p_pListResult)
{
	m_pMapRules = p_pRules;
	m_pArrayConstants = p_pConstants;
	m_ppStart = p_ppStart;
	m_pListResult = p_pListResult;
}

//*************************************************************************************************

LSystem :: LSystem( int p_width, int p_height, int p_iWndId, wxFrame * p_pFrame)
	:	ProceduralGenerator( p_width, p_height, p_iWndId, p_pFrame)
	,	m_pStart( NULL)
{
	m_bNeedsConfigPanel = true;
	Random::srand ( time( NULL));

	LSystem::Thread * l_pThread;
	int l_iOffset = m_iHeight / m_uiThreadCount;

	for (int i = 0 ; i < m_iHeight ; i += l_iOffset)
	{
		l_pThread = CreateThread<LSystem::Thread>( m_iWidth, i, i + l_iOffset, m_iHeight, m_pxColour);
		l_pThread->SetSystem( & m_mapRules, & m_arrayConstants, & m_pStart, & m_listResult);
		l_pThread->Run();
	}

	m_initialised = true;
}

LSystem :: ~LSystem()
{
	m_initialised = false;

	_cleanup();
}

//******************************************************************************

IMPLEMENT_PLUGIN( LsPlugin, LsPanel, LSystem)

LsPlugin :: LsPlugin()
{
}

LsPlugin :: ~LsPlugin()
{
}

wxString LsPlugin :: GetName()
{
	return wxT( "L-System");
}

//*************************************************************************************************

LsPanel :: LsPanel( wxWindow * p_pWindow, wxWindowID p_id, const wxPoint & p_ptPosition, const wxSize & p_size)
	:	ConfigPanel( p_pWindow, p_id, p_ptPosition, p_size)
{
}

LsPanel :: ~LsPanel()
{
}

void LsPanel :: Create( LSystem * p_pGenerator, wxWindow * p_pParent, const wxPoint & p_ptPosition, const wxSize & p_size)
{
	SetSize( p_ptPosition.x, p_ptPosition.y, p_size.x, p_size.y);
	Reparent( p_pParent);
	m_pGenerator = p_pGenerator;

	wxString l_strValue;
	l_strValue << int( m_pGenerator->GetRed());
	new wxStaticText( this, wxID_ANY, wxT( "R"), p_ptPosition, wxSize( 20, 16));
	m_pRedText = new wxStaticText( this, wxID_ANY, l_strValue, wxPoint( 20, 16), wxSize( 20, 16));
	m_pRedValue = new wxSlider( this, eRedValue, m_pGenerator->GetRed(), 0, 255, wxPoint( 20, 32), wxSize( 20, 60), wxSL_VERTICAL);

	l_strValue.clear();
	l_strValue << int( m_pGenerator->GetGreen());
	new wxStaticText( this, wxID_ANY, wxT( "G"), wxPoint( 60, 0), wxSize( 20, 16));
	m_pGreenText = new wxStaticText( this, wxID_ANY, l_strValue, wxPoint( 60, 16), wxSize( 20, 16));
	m_pGreenValue = new wxSlider( this, eGreenValue, m_pGenerator->GetGreen(), 0, 255, wxPoint( 60, 32), wxSize( 20, 60), wxSL_VERTICAL);

	l_strValue.clear();
	l_strValue << int( m_pGenerator->GetBlue());
	new wxStaticText( this, wxID_ANY, wxT( "B"), wxPoint( 100, 0), wxSize( 20, 16));
	m_pBlueText = new wxStaticText( this, wxID_ANY, l_strValue, wxPoint( 100, 16), wxSize( 20, 16));
	m_pBlueValue = new wxSlider( this, eBlueValue, m_pGenerator->GetBlue(), 0, 255, wxPoint( 100, 32), wxSize( 20, 60), wxSL_VERTICAL);
}

BEGIN_EVENT_TABLE( LsPanel, ConfigPanel)
	EVT_SLIDER( eRedValue,		LsPanel::_onSliderRed)
	EVT_SLIDER( eGreenValue,	LsPanel::_onSliderGreen)
	EVT_SLIDER( eBlueValue,		LsPanel::_onSliderBlue)
END_EVENT_TABLE()

void LsPanel :: _onSliderRed( wxCommandEvent & p_event)
{
	m_pGenerator->SetRed( p_event.GetInt());
	wxString l_strValue;
	l_strValue << p_event.GetInt();
	m_pRedText->SetLabel( l_strValue);
}

void LsPanel :: _onSliderGreen( wxCommandEvent & p_event)
{
	m_pGenerator->SetGreen( p_event.GetInt());
	wxString l_strValue;
	l_strValue << p_event.GetInt();
	m_pGreenText->SetLabel( l_strValue);
}

void LsPanel :: _onSliderBlue( wxCommandEvent & p_event)
{
	m_pGenerator->SetBlue( p_event.GetInt());
	wxString l_strValue;
	l_strValue << p_event.GetInt();
	m_pBlueText->SetLabel( l_strValue);
}

//*************************************************************************************************
