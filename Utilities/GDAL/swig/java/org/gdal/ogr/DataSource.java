/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 1.3.27
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package org.gdal.ogr;

import org.gdal.osr.SpatialReference;

public class DataSource {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  protected DataSource(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(DataSource obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public void delete() {
    if(swigCPtr != 0 && swigCMemOwn) {
      swigCMemOwn = false;
      ogrJNI.delete_DataSource(swigCPtr);
    }
    swigCPtr = 0;
  }

  public String getName() {
    return ogrJNI.DataSource_name_get(swigCPtr);
  }

  public int GetRefCount() {
    return ogrJNI.DataSource_GetRefCount(swigCPtr);
  }

  public int GetSummaryRefCount() {
    return ogrJNI.DataSource_GetSummaryRefCount(swigCPtr);
  }

  public int GetLayerCount() {
    return ogrJNI.DataSource_GetLayerCount(swigCPtr);
  }

  public Driver GetDriver() {
    long cPtr = ogrJNI.DataSource_GetDriver(swigCPtr);
    return (cPtr == 0) ? null : new Driver(cPtr, false);
  }

  public String GetName() {
    return ogrJNI.DataSource_GetName(swigCPtr);
  }

  public int DeleteLayer(int index) {
    return ogrJNI.DataSource_DeleteLayer(swigCPtr, index);
  }

  public Layer CreateLayer(String name, SpatialReference reference, int geom_type, java.util.Vector options) {
    long cPtr = ogrJNI.DataSource_CreateLayer(swigCPtr, name, SpatialReference.getCPtr(reference), geom_type, options);
    return (cPtr == 0) ? null : new Layer(cPtr, false);
  }

  public Layer CopyLayer(Layer src_layer, String new_name, java.util.Vector options) {
    long cPtr = ogrJNI.DataSource_CopyLayer(swigCPtr, Layer.getCPtr(src_layer), new_name, options);
    return (cPtr == 0) ? null : new Layer(cPtr, false);
  }

  public Layer GetLayerByIndex(int index) {
    long cPtr = ogrJNI.DataSource_GetLayerByIndex(swigCPtr, index);
    return (cPtr == 0) ? null : new Layer(cPtr, false);
  }

  public Layer GetLayerByName(String layer_name) {
    long cPtr = ogrJNI.DataSource_GetLayerByName(swigCPtr, layer_name);
    return (cPtr == 0) ? null : new Layer(cPtr, false);
  }

  public boolean TestCapability(String cap) {
    return ogrJNI.DataSource_TestCapability(swigCPtr, cap);
  }

  public Layer ExecuteSQL(String statement, Geometry geom, String dialect) {
    long cPtr = ogrJNI.DataSource_ExecuteSQL(swigCPtr, statement, Geometry.getCPtr(geom), dialect);
    return (cPtr == 0) ? null : new Layer(cPtr, true);
  }

  public void ReleaseResultSet(Layer layer) {
    ogrJNI.DataSource_ReleaseResultSet(swigCPtr, Layer.getCPtr(layer));
  }

}
