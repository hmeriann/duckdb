SELECT 
  list_aggregate(tau_pt, 'stddev'),
  list_aggregate(tau_eta, 'stddev'),
  list_aggregate(jet_pt, 'stddev'),
  list_aggregate(jet_eta, 'stddev'),
  list_aggregate(muon_pt, 'stddev'),
  list_aggregate(muon_eta, 'stddev'),
  list_aggregate(el_pt, 'stddev'),
  list_aggregate(el_eta, 'stddev'),
  list_aggregate(ph_pt, 'stddev'),
  list_aggregate(ph_eta, 'stddev')  
FROM singleMu;