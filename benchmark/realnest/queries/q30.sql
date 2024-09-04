SELECT 
  list_aggregate(tau_pt, 'stddev_samp'),
  list_aggregate(tau_eta, 'stddev_samp'),
  list_aggregate(jet_pt, 'stddev_samp'),
  list_aggregate(jet_eta, 'stddev_samp'),
  list_aggregate(muon_pt, 'stddev_samp'),
  list_aggregate(muon_eta, 'stddev_samp'),
  list_aggregate(el_pt, 'stddev_samp'),
  list_aggregate(el_eta, 'stddev_samp'),
  list_aggregate(ph_pt, 'stddev_samp'),
  list_aggregate(ph_eta, 'stddev_samp')  
FROM singleMu;