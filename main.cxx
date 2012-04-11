#include <iostream>

#include <itkImage.h>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkDanielssonDistanceMapImageFilter.h"


int main( int argc, char * argv[] )
{
  if( argc < 5 )
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputDistanceMapImageFile ";
    std::cerr << " outputVoronoiMapImageFile ";
    std::cerr << " outputVectorMapImageFile ";
    std::cerr << std::endl;  
    return EXIT_FAILURE;
    }

  typedef  unsigned char   InputPixelType;
  typedef  unsigned short  OutputPixelType;
  typedef itk::Image< InputPixelType,  3 >   InputImageType;
  typedef itk::Image< OutputPixelType, 3 >   OutputImageType;

  typedef itk::DanielssonDistanceMapImageFilter< InputImageType, OutputImageType >  FilterType;
  FilterType::Pointer filter = FilterType::New();

  typedef itk::RescaleIntensityImageFilter< OutputImageType, OutputImageType > RescalerType;
  RescalerType::Pointer scaler = RescalerType::New();

  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );

  filter->SetInput( reader->GetOutput() );
  scaler->SetInput( filter->GetOutput() );
  writer->SetInput( scaler->GetOutput() );


  scaler->SetOutputMaximum( 65535L );
  scaler->SetOutputMinimum(     0L );

  filter->InputIsBinaryOn();

  try
    {
      writer->Update();
    }
  catch( itk::ExceptionObject exp )
    {
      std::cerr << "Exception caught !" << std::endl;
      std::cerr <<     exp    << std::endl;
    }

  const char * voronoiMapFileName = argv[3];

  scaler->SetInput( filter->GetVoronoiMap() );
  writer->SetFileName( voronoiMapFileName );
  try
    {
      writer->Update();
    }
  catch( itk::ExceptionObject exp )
    {
      std::cerr << "Exception caught !" << std::endl;
      std::cerr <<     exp    << std::endl;
    }

  typedef FilterType::VectorImageType   OffsetImageType;

  typedef itk::ImageFileWriter< OffsetImageType >  WriterOffsetType;
  WriterOffsetType::Pointer offsetWriter = WriterOffsetType::New();

  offsetWriter->SetInput(  filter->GetVectorDistanceMap()  );

  offsetWriter->SetFileName( argv[4]  );

  try
    {
      offsetWriter->Update();
    }
  catch( itk::ExceptionObject exp )
    {
      std::cerr << "Exception caught !" << std::endl;
      std::cerr <<     exp    << std::endl;
    }

  return EXIT_SUCCESS;
}

