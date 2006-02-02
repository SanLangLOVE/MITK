#include "mitkBaseXMLWriter.h"

#include <fstream>
#include <string>
#include <stack>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>

namespace mitk {


BaseXMLWriter::BaseXMLWriter( const char* filename, int space )
:m_Out(NULL), m_Increase(0), m_Space(space), m_NodeCount(0) , m_File(true), m_FirstNode(true), m_XMLPathAndFilename(filename)

{		
	m_Out = new std::ofstream( filename );			
}


//BaseXMLWriter::BaseXMLWriter( std::ostream& out, int space )
//:m_Out(&out), m_Increase(0), m_Space(space), m_NodeCount(0), m_File(false), m_FirstNode(true) 
//{}


BaseXMLWriter::~BaseXMLWriter() 
{
  if ( !m_Out )
    return;

  m_Out->flush();

	if ( m_File ) 
	{
		std::ofstream* file = static_cast<std::ofstream*>(m_Out);
		file->close();
		delete file;
	}
}

/**
  begins a new XML node

  \ param name specifies the name of the XML node
*/
void BaseXMLWriter::BeginNode( const std::string& name ) 
{		
	if ( m_FirstNode ) 
	{
		*m_Out << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
		m_FirstNode = false;
	}

  m_Stack.push( new StreamNode( name ) );
  
	m_Increase++;
	m_NodeCount++;
}


void BaseXMLWriter::EndNode( )
{			
	m_Increase--;

  int steps = m_Space * m_Increase;
  StreamNode* current = m_Stack.top();
  m_Stack.pop();

  if ( m_Stack.size() )
  {
    std::ostream& stream = m_Stack.top()->GetChildrenStream();
    current->Write( stream, steps );      
    delete current;
  }
  else
  {
    current->Write( *m_Out, steps );
  }
}

/**
  writes a XML attribute that datatype is a const char*

  \ param key specifies the name of the attribute
  \ param value represents the data of the attribute
*/
void BaseXMLWriter::WriteProperty( const std::string& key, const char* value ) const
{
  std::ostream& stream = m_Stack.top()->GetPropertyStream();
  stream << ConvertString( key.c_str() );
	stream << "=\"" << ConvertString( value ) << "\" ";
}


void BaseXMLWriter::WriteProperty( const std::string& key, const std::string& value ) const 
{
  std::ostream& stream = m_Stack.top()->GetPropertyStream();
	stream << ConvertString( key.c_str() );
	stream << "=\"" << ConvertString( value.c_str() ) << "\" ";
}


void BaseXMLWriter::WriteProperty( const std::string& key, int value ) const 
{
  std::ostream& stream = m_Stack.top()->GetPropertyStream();
	stream << ConvertString( key.c_str() );
	stream << "=\"" << value << "\" ";
}


void BaseXMLWriter::WriteProperty( const std::string& key, float value ) const
{
  std::ostream& stream = m_Stack.top()->GetPropertyStream();
	stream << ConvertString( key.c_str() );
	stream << "=\"" << value << "\" ";
}


void BaseXMLWriter::WriteProperty( const std::string& key, double value ) const
{
  std::ostream& stream = m_Stack.top()->GetPropertyStream();
	stream << ConvertString( key.c_str() );
	stream << "=\"" << value << "\" ";
}


void BaseXMLWriter::WriteProperty( const std::string& key, bool value ) const
{
  std::ostream& stream = m_Stack.top()->GetPropertyStream();
	stream << ConvertString( key.c_str() );

  if ( value == true )
	  stream << "=\"" << "TRUE\" ";
  else
    stream << "=\"" << "FALSE\" ";
}


void BaseXMLWriter::WriteComment( const std::string& text ) 
{
  std::ostream& stream = m_Stack.top()->GetComment();

	if ( m_FirstNode ) 
	{
		stream <<"<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n";
		m_FirstNode = false;
	}

  stream << ConvertString( text.c_str() );
}
	

int BaseXMLWriter::GetCurrentDeph() const
{
	return m_Stack.size();
}


int BaseXMLWriter::GetNodeCount() const
{
	return m_NodeCount;
}


int BaseXMLWriter::GetSpace() const
{
	return m_Space;
}


void BaseXMLWriter::SetSpace( int space )
{
	m_Space = space;
}


const char* BaseXMLWriter::ConvertString( const char* string ) const
{
	static std::char_traits<char>::char_type buffer[255];
	int length = std::char_traits<char>::length( string );
	std::char_traits<char>::copy ( buffer, string, length + 1 );
	const char* pos = buffer;
	
	while ( pos != NULL ) 
	{
		pos = std::char_traits<char>::find ( buffer, length , '<');

		if( pos != NULL )
			*(const_cast<char*>(pos)) = '{';
	}

	pos = buffer;
	
	while ( pos != NULL ) 
	{
		pos = std::char_traits<char>::find ( buffer, length , '>');

		if( pos != NULL )
			*(const_cast<char*>(pos)) = '}';
	}

	return buffer;
}


void BaseXMLWriter::StreamNode::Write( std::ostream& out, int steps )
{
  std::string comment = m_Comment.str();

  if ( comment.size() )
  {
    for(int i=0; i < steps; i++) out << ' '; 
    out << "<!-- " << comment << " -->\n";
  }

	for(int i=0; i < steps; i++) out << ' '; 

  out << '<' << m_Name;

  std::string properties = m_PropertyStream.str();

  if ( properties.size() )
    out << ' ' << properties;

  std::string children = m_ChildrenStream.str();

  if ( children.size() )
  {
    out << ">\n";
    out << children;
    for(int i=0; i < steps; i++) out << ' '; 
    out << "</" << m_Name << ">\n";
  }
  else
  {
    out << "/>\n";
  }
}


const std::string BaseXMLWriter::GetXMLPathAndFileName()
{
  return m_XMLPathAndFilename;
}

} // namespace mitk

