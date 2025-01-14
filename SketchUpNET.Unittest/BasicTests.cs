﻿using System;
using System.Linq;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace SketchUpNET.Unittest
{
    [TestClass]
    public class UnitTests
    {
        [TestInitialize()]
        public void Initialize()
        {
            Type t = typeof(UnitTests);
            TestFile = System.IO.Path.GetDirectoryName(t.Assembly.Location) + @"\..\..\..\..\Testfiles\TestModel.skp";
            TestFile = @"C:\Users\user\Documents\测试属性.skp";
        }

        public static string TestFile;

        /// <summary>
        /// Test loading surfaces and meshes from testfile
        /// </summary>
        [TestMethod]
        public void TestGetMesh()
        {
            SketchUpNET.SketchUp skp = new SketchUp();
            skp.LoadModel(TestFile, true);
            foreach (var srf in skp.Surfaces)
            {
                Assert.IsNotNull(srf.FaceMesh);
                Assert.IsTrue(srf.FaceMesh.Faces.Count > 0);
                Assert.IsTrue(srf.FaceMesh.Vertices.Count > 0);
            }
        }

        /// <summary>
        /// Test loading surfaces and meshes from testfile
        /// </summary>
        [TestMethod]
        public void TestGetAxis()
        {
            SketchUpNET.SketchUp skp = new SketchUp();
            skp.LoadModel(TestFile, true);
            Assert.IsNotNull(skp.Axis);
            Assert.IsTrue(Math.Abs(skp.Axis.Origin.X) > 0);
            Assert.IsTrue(Math.Abs(skp.Axis.Origin.Y) > 0);
            Assert.IsTrue(Math.Abs(skp.Axis.Origin.Z) == 0);
            Assert.IsNotNull(skp.Axis.Origin);
            Assert.IsNotNull(skp.Axis.AxisTransform);
        }

        /// <summary>
        /// Test loading surfaces and meshes from testfile
        /// </summary>
        [TestMethod]
        public void TestHiddenGeo()
        {
            SketchUpNET.SketchUp skp = new SketchUp();
            skp.LoadModel(TestFile, true);
            var hiddengroups = skp.Groups.Where(x => x.Visable == false).ToList();
            var showgroups = skp.Groups.Where(x => x.Visable == true).ToList();
            Assert.AreEqual(1, hiddengroups.Count);
            Assert.AreEqual(0, showgroups.Count);
            var hiddeninstances = skp.Instances.Where(x => x.Visable == false).ToList();
            var showginstances = skp.Instances.Where(x => x.Visable == true).ToList();
            Assert.AreEqual(0, hiddeninstances.Count);
            Assert.AreEqual(2, showginstances.Count);
        }

        /// <summary>
        /// Test saving file as
        /// </summary>
        [TestMethod]
        public void TestSaveAs()
        {
            SketchUpNET.SketchUp skp = new SketchUp();
            string dir = System.IO.Path.GetDirectoryName(TestFile);
            skp.SaveAs(TestFile, SKPVersion.V2016, dir + "/NewFile.skp");
            Assert.IsTrue(System.IO.File.Exists(dir + "/NewFile.skp"));
        }

        /// <summary>
        /// Test saving UTF8 Filenames
        /// </summary>
        [TestMethod]
        public void TestSaveAsUTF8()
        {
            SketchUpNET.SketchUp skp = new SketchUp();
            string dir = System.IO.Path.GetDirectoryName(TestFile);
            skp.SaveAs(TestFile, SKPVersion.V2016, dir + "/Überß.skp");

            Assert.IsTrue(System.IO.File.Exists(dir + "/Überß.skp"));
            bool res = skp.LoadModel(dir + "/Überß.skp");
            Assert.IsTrue(res);
        }

        /// <summary>
        /// Test loading model without meshes
        /// </summary>
        [TestMethod]
        public void TestDoNotGetMesh()
        {
            SketchUpNET.SketchUp skp = new SketchUp();
            skp.LoadModel(TestFile, false);
            foreach (var srf in skp.Surfaces)
            {
                Assert.IsNull(srf.FaceMesh);
            }
        }

        /// <summary>
        /// Test getting Materials from model
        /// </summary>
        [TestMethod]
        public void TestGetMaterial()
        {
            SketchUpNET.SketchUp skp = new SketchUp();
            skp.LoadModel(TestFile, false);

            bool found = false;

            foreach (var srf in skp.Surfaces)
            {
                Assert.IsNotNull(srf.BackMaterial);
                Assert.IsNotNull(srf.FrontMaterial);
                Assert.IsNotNull(srf.BackMaterial.Colour);
                Assert.IsNotNull(srf.FrontMaterial.Colour);
                if (srf.BackMaterial.Name == "MyMat" || srf.FrontMaterial.Name == "MyMat")
                    found = true;
            }

            Assert.IsTrue(found);
        }
    }
}
