package com.example.perfomance_test

import android.content.res.AssetManager
import android.os.Bundle
import android.view.MenuItem
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.fragment.NavHostFragment
import androidx.navigation.ui.navigateUp
import androidx.viewpager.widget.ViewPager
import com.example.perfomance_test.databinding.ActivityMainBinding
import com.example.perfomance_test.ui.main.SectionsPagerAdapter
import com.google.android.material.tabs.TabLayout
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.io.OutputStream

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        getSupportActionBar()?.hide();

        val sectionsPagerAdapter = SectionsPagerAdapter(this, supportFragmentManager)
        val viewPager: ViewPager = binding.viewPager
        viewPager.adapter = sectionsPagerAdapter
        val tabs: TabLayout = binding.tabs
        tabs.setupWithViewPager(viewPager)

        val extFilesPath = getExternalFilesDir("")
        assets.copyAssetFolder("", extFilesPath!!.absolutePath)

        initializeTestEnvironment(assets, extFilesPath!!.absolutePath)
    }

    fun AssetManager.copyAssetFolder(srcName: String, dstName: String): Boolean {
        return try {
            var result = true
            val fileList = this.list(srcName) ?: return false
            if (fileList.isEmpty()) {
                result = copyAssetFile(srcName, dstName)
            } else {
                val file = File(dstName)
                result = file.mkdirs()
                for (filename in fileList) {
                    if(srcName.isEmpty())
                        result = result and copyAssetFolder(
                            filename,
                            dstName + File.separator.toString() + filename
                        )
                    else
                        result = result and copyAssetFolder(
                            srcName + File.separator.toString() + filename,
                            dstName + File.separator.toString() + filename
                        )
                }
            }
            result
        } catch (e: IOException) {
            e.printStackTrace()
            false
        }
    }

    fun AssetManager.copyAssetFile(srcName: String, dstName: String): Boolean {
        return try {
            val inStream = this.open(srcName)
            val outFile = File(dstName)
            val out: OutputStream = FileOutputStream(outFile)
            val buffer = ByteArray(1024)
            var read: Int
            while (inStream.read(buffer).also { read = it } != -1) {
                out.write(buffer, 0, read)
            }
            inStream.close()
            out.close()
            true
        } catch (e: IOException) {
            e.printStackTrace()
            false
        }
    }

    external fun initializeTestEnvironment(assetManager: AssetManager, writablePath: String)

    companion object {
        // Used to load the 'perfomance_test' library on application startup.
        init {
            System.loadLibrary("perfomance_test")
        }
    }
}