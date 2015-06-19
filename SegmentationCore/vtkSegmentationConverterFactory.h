/*==============================================================================

  Copyright (c) Laboratory for Percutaneous Surgery (PerkLab)
  Queen's University, Kingston, ON, Canada. All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Andras Lasso, PerkLab, Queen's University
  and was supported through the Applied Cancer Research Unit program of Cancer Care
  Ontario with funds provided by the Ontario Ministry of Health and Long-Term Care

==============================================================================*/

#ifndef __vtkSegmentationConverterFactory_h
#define __vtkSegmentationConverterFactory_h

#include "vtkSegmentationCoreConfigure.h"

// VTK includes
#include <vtkObject.h>
#include <vtkSmartPointer.h>

// STD includes
#include <vector>

class vtkSegmentationConverterRule;
class vtkDataObject;

/// \ingroup SegmentationCore
/// \brief Class that can create vtkSegmentationConverter instances.
///
/// This singleton class is a repository of all segmentation converter rules.
/// Singleton pattern adopted from vtkEventBroker class.
class vtkSegmentationCore_EXPORT vtkSegmentationConverterFactory : public vtkObject
{
public:
  typedef std::vector< vtkSmartPointer<vtkSegmentationConverterRule> > RuleListType;

  vtkTypeMacro(vtkSegmentationConverterFactory, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  /// Create a copy of all registered converter rules.
  /// The rule argument is overwritten (any previous content is cleared) with rules
  /// copied from the list of all registered rules.
  void CopyConverterRules(RuleListType &rules);

  /// Add a converter rule.
  /// The factory (and all converter classes it creates) will keep a references to this object,
  /// and it will not be deleted until all these referring classes are deleted.
  void RegisterConverterRule(vtkSegmentationConverterRule* rule);

  void UnregisterConverterRule(vtkSegmentationConverterRule* rule);

  /// Get all registered converter rules
  const RuleListType& GetConverterRules();

  /// Constructs representation object from class name using the ConstructRepresentationObject
  /// methods in the registered conversion rules that must be able to instantiate the representation
  /// classes they support.
  vtkDataObject* ConstructRepresentationObjectByClass(std::string className);

  /// Constructs representation object from representation name using the ConstructRepresentationObject
  /// methods in the registered conversion rules that must be able to instantiate the representation
  /// classes they support.
  vtkDataObject* ConstructRepresentationObjectByRepresentation(std::string representationName);

  /// Return the singleton instance with no reference counting.
  static vtkSegmentationConverterFactory* GetInstance();
  
  /// This is a singleton pattern New.  There will only be ONE
  /// reference to a vtkSegmentationConverterFactory object per process.  Clients that
  /// call this must call Delete on the object so that the reference
  /// counting will work. The single instance will be unreferenced when
  /// the program exits.
  static vtkSegmentationConverterFactory* New();

protected:
  vtkSegmentationConverterFactory();
  ~vtkSegmentationConverterFactory();
  vtkSegmentationConverterFactory(const vtkSegmentationConverterFactory&);
  void operator=(const vtkSegmentationConverterFactory&);

  // Singleton management functions.
  static void classInitialize();
  static void classFinalize();

  friend class vtkSegmentationConverterFactoryInitialize;
  typedef vtkSegmentationConverterFactory Self;

  /// Registered converter rules
  RuleListType Rules;

private:
  /// DetachObservations is a fast (but dangerous) method to delete all the
  /// observations. It leaves the event broker in an inconsistent state:
  ///  - SubjectMap and ObserverMap are not being updated.
  void DetachObservations();
  /// vtkObservation can call these methods
  friend class vtkObservation;
};

/// Utility class to make sure qSlicerModuleManager is initialized before it is used.
class vtkSegmentationCore_EXPORT vtkSegmentationConverterFactoryInitialize
{
public:
  typedef vtkSegmentationConverterFactoryInitialize Self;

  vtkSegmentationConverterFactoryInitialize();
  ~vtkSegmentationConverterFactoryInitialize();
private:
  static unsigned int Count;
};

/// This instance will show up in any translation unit that uses
/// vtkSegmentationConverterFactory.  It will make sure vtkSegmentationConverterFactory is initialized
/// before it is used.
static vtkSegmentationConverterFactoryInitialize vtkSegmentationConverterFactoryInitializer;

#endif