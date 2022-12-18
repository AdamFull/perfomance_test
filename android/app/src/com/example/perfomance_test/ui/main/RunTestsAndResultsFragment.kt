package com.example.perfomance_test.ui.main

import android.os.Bundle
import android.view.LayoutInflater
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import androidx.activity.OnBackPressedDispatcher
import androidx.appcompat.app.AppCompatActivity
import androidx.appcompat.widget.Toolbar
import androidx.fragment.app.Fragment
import androidx.fragment.app.FragmentManager
import androidx.navigation.fragment.NavHostFragment
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.NavigationUI
import androidx.navigation.ui.NavigationUI.setupActionBarWithNavController
import com.example.perfomance_test.R
import com.example.perfomance_test.databinding.FragmentRunTestsAndResultsBinding

/**
 * A simple [Fragment] subclass.
 * Use the [RunTestsAndResultsFragment.newInstance] factory method to
 * create an instance of this fragment.
 */
class RunTestsAndResultsFragment : Fragment() {
    private var binding: FragmentRunTestsAndResultsBinding? = null
    private lateinit var appBarConfiguration: AppBarConfiguration

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun onCreateView(
        inflater: LayoutInflater, container: ViewGroup?,
        savedInstanceState: Bundle?
    ): View? {
        binding = FragmentRunTestsAndResultsBinding.inflate(inflater, container, false)
        return binding!!.root
    }

    override fun onViewCreated(view: View, savedInstanceState: Bundle?) {
        // Define container.
        val childNavController = childFragmentManager.findFragmentById(R.id.nav_host_fragment_content_main) as NavHostFragment

        val toolbar = view.findViewById<Toolbar>(R.id.toolbar)
        (activity as AppCompatActivity?)!!.setSupportActionBar(toolbar)

        if (childNavController != null) {
            val navController = childNavController.navController
            appBarConfiguration = AppBarConfiguration(navController.graph)
            toolbar.setNavigationOnClickListener {_ ->
                NavigationUI.navigateUp(navController, appBarConfiguration)
            }
            setupActionBarWithNavController(requireActivity() as AppCompatActivity, navController, appBarConfiguration)
        }
    }
}