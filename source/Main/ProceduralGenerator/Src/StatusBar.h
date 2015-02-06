#ifndef ___PG_StatusBar___
#define ___PG_StatusBar___

namespace ProceduralTextures
{
	class StatusBar
		: public wxStatusBar
	{
	private:
		wxStaticText * m_pTxtCtrls[2];

	public:
		StatusBar( wxWindow * p_pParent, wxWindowID p_id = wxID_ANY, long p_lStyle = wxST_SIZEGRIP | wxFULL_REPAINT_ON_RESIZE, const wxString & p_strName = wxStatusBarNameStr );
		virtual ~StatusBar();

		virtual void SetStatusText( const wxString & p_strText, int p_iNumber = 0 );

	protected:
		DECLARE_EVENT_TABLE();
		void _onSize( wxSizeEvent & p_event );
	};
}

#endif
